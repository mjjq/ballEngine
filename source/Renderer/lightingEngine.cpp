#include "lightingEngine.h"
#include "math.h"
#include <cmath>

LightingEngine::LightingEngine()
{}


void LightingEngine::clearShadowTextures(sf::View const & view)
{
    for(int i=0; i<(int)shadowTextures.size(); ++i)
    {
        shadowTextures[i]->setView(view);
        shadowTextures[i]->clear(sf::Color::White);
    }
}

void LightingEngine::displayShadowTextures()
{
    for(int i=0; i<(int)shadowTextures.size(); ++i)
        shadowTextures[i]->display();
}

void LightingEngine::generateShadowTextures(std::vector<Renderable* > const & _renderObjects)
{
    for(int i=0; i<(int)lights.size(); ++i)
    {
        for(int j=0; j<(int)_renderObjects.size(); ++j)
        {

            shadowStencil(*lights[i], *_renderObjects[j]->primShape, *shadowTextures[i]);

            std::string shadowTextureString = "shadowTextures[" + std::to_string(i) + "]";
            if(_renderObjects[j]->shader != nullptr)
                _renderObjects[j]->shader->setUniform(shadowTextureString,
                                                      shadowTextures[i]->getTexture());
        }
    }

    /*if(shadowTextures.size()>0)
    {
        sf::Sprite shape(shadowTextures[0]->getTexture());
        sf::Sprite shape2(lights[0]->umbralTexture.getTexture());
        shape2.setPosition({0.0f, 1000.0f});
        windowManager.getWindow().draw(shape);
        windowManager.getWindow().draw(shape2);
    }*/
}


void LightingEngine::shadowStencil(LightSource &lightSource,
                                   sf::Shape &shape,
                                   sf::RenderTexture &shadowTexture)
{
    using Math::orthogonal;
    using Math::dot;
    using Math::fastNorm;
    using Math::modulo;

    std::map<int, Ray > stencilRays;
    int noStencilBounds = 0;
    int smallestKey = 100000000;
    sf::Vector2f lightPos = {lightSource.position.x,
                             lightSource.position.y};

    sf::Transform trans = shape.getTransform();

    int n=shape.getPointCount();
    std::vector<sf::Vector2f > shapePoints;
    for(int i=0; i<(int)n; ++i)
    {
        shapePoints.push_back(trans.transformPoint(shape.getPoint(i)));
    }

    for(int i=0; i<(int)n; ++i)
    {
        sf::Vector2f shapePos0 = shapePoints[modulo((i-2),n)];
        sf::Vector2f shapePos1 = shapePoints[modulo((i-1),n)];
        sf::Vector2f shapePos2 = shapePoints[i];
        sf::Vector2f shapePos3 = shapePoints[modulo((i+1),n)];

        sf::Vector2f n1 = orthogonal(shapePos1 - shapePos0, 1.0);
        sf::Vector2f n2 = orthogonal(shapePos2 - shapePos1, 1.0);
        sf::Vector2f n3 = orthogonal(shapePos3 - shapePos2, 1.0);

        sf::Vector2f lightDir1 = shapePos1 - lightPos;
        sf::Vector2f lightDir2 = shapePos2 - lightPos;
        sf::Vector2f lightDir3 = shapePos3 - lightPos;

        if(dot(lightDir2, n2) <= 0.0)
        {
            if(dot(lightDir1, n1) > 0.00001)
            {
                ++noStencilBounds;
                stencilRays.insert({modulo(i-1, n), Ray(shapePos1, fastNorm(lightDir1), 1.0f)});
            }
            if(dot(lightDir3, n3) > 0.00001)
            {
                ++noStencilBounds;
                stencilRays.insert({i, Ray(shapePos2, fastNorm(lightDir2), 1.0f)});

                if(i < smallestKey)
                    smallestKey = i;
            }
        }
        else
        {
            stencilRays.insert({i, Ray(shapePos2, fastNorm(lightDir2), 1.0f)});
        }
    }

    if(noStencilBounds == 2)
    {
        float minCosine = 1e+15;
        float maxCosine = -minCosine;
        sf::VertexArray result(sf::LineStrip, stencilRays.size());

        int sRSize = stencilRays.size();
        Ray& firstRay = stencilRays[modulo(smallestKey, n)];
        Ray& lastRay = stencilRays[modulo(smallestKey+sRSize-1, n)];

        for(int j=0; j<sRSize; ++j)
        {
            Ray& temp = stencilRays[modulo(j+smallestKey, n)];

            result[j].position = temp.pos;

            float cosine = dot(Math::fastNorm(temp.pos - lightPos),
                               lastRay.dir - firstRay.dir);
            if(cosine < minCosine) minCosine = cosine;
            if(cosine > maxCosine) maxCosine = cosine;

            result[j].texCoords = {cosine, 0.0f};
        }

        //scale texCoord to fit range [0,1]
        for(int j=0; j<sRSize; ++j)
        {
            result[j].texCoords.x = (result[j].texCoords.x - minCosine) /
                                        (maxCosine - minCosine);
        }

        float distanceLeftSq = Math::square(firstRay.pos - lightPos);
        float distanceRightSq = Math::square(lastRay.pos - lightPos);
        float angularDiameter = lightSource.lightProperties.umbralRadius/
                                acosf(Math::dot(firstRay.dir, lastRay.dir));

        umbralShader->setUniform("lightWidthL", angularDiameter/sqrtf(distanceLeftSq));
        umbralShader->setUniform("lightWidthR", angularDiameter/sqrtf(distanceRightSq));
        umbralShader->setUniform("lightPos", sf::Glsl::Vec3(lightPos.x, lightPos.y, 0.0f));
        umbralShader->setUniform("rayLength", lightSource.effectiveRadius/
                                 sqrtf(Math::square(shape.getPosition()-lightPos)));

        shadowTexture.draw(result, umbralShader);
    }
}

bool LightingEngine::isShapeWithinRange(sf::Shape &shape, LightSource& lightSource)
{
    sf::Rect<float> shapeBounds = shape.getGlobalBounds();
    sf::Vector2f lightPos = {lightSource.position.x, lightSource.position.y};
    float lightRadiusSq = lightSource.effectiveRadius*
                          lightSource.effectiveRadius;

    if(Math::square(sf::Vector2f{shapeBounds.left, shapeBounds.top} - lightPos) < lightRadiusSq)
       return true;
    if(Math::square(sf::Vector2f{shapeBounds.left+shapeBounds.width, shapeBounds.top}-lightPos) < lightRadiusSq)
       return true;
    if(Math::square(sf::Vector2f{shapeBounds.left, shapeBounds.top+shapeBounds.height}-lightPos) < lightRadiusSq)
       return true;
    if(Math::square(sf::Vector2f{shapeBounds.left+shapeBounds.width, shapeBounds.top+shapeBounds.height}-lightPos) < lightRadiusSq)
       return true;

    return false;
}

void LightingEngine::addLightSource(LightSource* light)
{
    lights.push_back(light);

    sf::RenderTexture* texture = new sf::RenderTexture;
    texture->create(shadowTextureSize.x, shadowTextureSize.y);
    shadowTextures.emplace_back(texture);
}

void LightingEngine::removeLightSource(LightSource* light)
{
    for(int i=0; i<(int)lights.size(); ++i)
        if(lights[i] == light)
        {
            lights.erase(lights.begin() + i);

            sf::RenderTexture* tToDelete = shadowTextures[i];
            shadowTextures.erase(shadowTextures.begin() + i);
            delete tToDelete;
        }
}

void LightingEngine::resizeTextures(sf::Vector2u const & size)
{
    shadowTextureSize = size;
    for(int i=0; i<shadowTextures.size(); ++i)
    {
        shadowTextures[i]->create(shadowTextureSize.x, shadowTextureSize.y);
    }
}

void LightingEngine::setShader(sf::Shader *_shader)
{
    umbralShader = _shader;
}

const std::string LightingEngine::getShaderName()
{
    return umbralShaderName;
}

void LightingEngine::setObjectShaderLightProps(sf::Shader& objectShader)
{
    for(int j=0; j<10; ++j)
    {
        std::string lightVal = "lights[" + std::to_string(j) + "]";
        std::string shadowVal = "shadowTextures[" + std::to_string(j) + "]";
        if(j < (int)lights.size())
        {
            objectShader.setUniform("light.color",           lights[j]->lightProperties.color);
            objectShader.setUniform(lightVal + ".color",     lights[j]->lightProperties.color);
            objectShader.setUniform(lightVal + ".position",  lights[j]->position);
            objectShader.setUniform(lightVal + ".constant",  lights[j]->lightProperties.constant);
            objectShader.setUniform(lightVal + ".linear",    lights[j]->lightProperties.linear);
            objectShader.setUniform(lightVal + ".quadratic", lights[j]->lightProperties.quadratic);
            //shader->setUniform(shadowVal, &shadowTextures[j]->getTexture());
        }
        else
        {
            objectShader.setUniform(lightVal + ".color", sf::Vector3f(0.0,0.0,0.0));
            objectShader.setUniform(lightVal + ".position", sf::Vector3f(0.0,0.0,0.0));
            objectShader.setUniform(lightVal + ".constant", 1.0f);
            objectShader.setUniform(lightVal + ".linear", 0.0f);
            objectShader.setUniform(lightVal + ".quadratic", 0.0f);
            //shader->setUniform(shadowVal, &blankShadowTexture);
        }
    }
}
