#include "classLight.h"
#include "Math.h"
#include <iostream>
#include <cmath>
#include "stringConversion.h"

LightSource::LightSource(LightProperties properties) :
    lightProperties{properties}, position{properties.position}
{
    calcEffectiveRadius(5.0f/256.0f);

    if(umbralShader.loadFromFile("./res/shaders/umbralgen.vert",
                                 "./res/shaders/umbralgen.geom",
                                 "./res/shaders/umbralgen.frag"))
        umbralShader.setUniform("lightWidth", 0.5f);

    umbralTexture.create(500, 500);
    sf::VertexArray uTVerts(sf::TriangleStrip, 4);
    uTVerts[0].position = {0.0f, 0.0f};
    uTVerts[0].texCoords = {0.0f, 0.0f};
    uTVerts[1].position = {0.0f, 500.0f};
    uTVerts[1].texCoords = {0.0f, 1.0f};
    uTVerts[2].position = {500.0f, 0.0f};
    uTVerts[2].texCoords = {1.0f, 0.0f};
    uTVerts[3].position = {500.0f, 500.0f};
    uTVerts[3].texCoords = {1.0f, 1.0f};
    uTVerts[3].color = sf::Color::White;
    umbralTexture.draw(uTVerts, &umbralShader);
    umbralTexture.display();

    if(shadowShader == nullptr)
    {
        shadowShader = new sf::Shader();
        if(shadowShader->loadFromMemory(shadowShaderCode, sf::Shader::Fragment))
            shadowShader->setUniform("umbralTexture", umbralTexture.getTexture());
    }
    renderSubject.notify(*this, Event(EventType::New_LightSrc));
}

LightSource::~LightSource()
{
    renderSubject.notify(*this, Event(EventType::Delete_LightSrc));
}

sf::VertexArray LightSource::shadowStencil(sf::Shape &shape,
                                        sf::RenderTexture &shadowTexture)
{
    std::map<int, Ray > stencilRays;
    int noStencilBounds = 0;
    int smallestKey = 100000000;
    sf::Vector2f lightPos = {position.x, position.y};

    sf::Transform trans = shape.getTransform();

    int n=shape.getPointCount();
    std::vector<sf::Vector2f > shapePoints;
    for(int i=0; i<(int)n; ++i)
    {
        shapePoints.push_back(trans.transformPoint(shape.getPoint(i)));
    }

    for(int i=0; i<(int)n; ++i)
    {
        using Math::orthogonal;
        using Math::dot;
        using Math::norm;
        using Math::modulo;

        sf::Vector2f shapePos0 = shapePoints[modulo((i-2),n)];
        sf::Vector2f shapePos1 = shapePoints[modulo((i-1),n)];
        sf::Vector2f shapePos2 = shapePoints[modulo((i),n)];
        sf::Vector2f shapePos3 = shapePoints[modulo((i+1),n)];

        sf::Vector2f n1 = orthogonal(shapePos1 - shapePos0, 1.0);
        sf::Vector2f n2 = orthogonal(shapePos2 - shapePos1, 1.0);
        sf::Vector2f n3 = orthogonal(shapePos3 - shapePos2, 1.0);

        sf::Vector2f lightDir1 = lightPos - shapePos1;
        sf::Vector2f lightDir2 = lightPos - shapePos2;
        sf::Vector2f lightDir3 = lightPos - shapePos3;

        if(dot(lightDir2, n2) > 0.00001)
        {
            if(dot(lightDir1, n1) <= 0.0)
            {
                float tFactor = effectiveRadius - sqrtf(Math::square(lightDir1));
                if(tFactor > 0.0f)
                {
                    ++noStencilBounds;
                    stencilRays.insert({modulo(i-1, n), Ray(shapePos1, -norm(lightDir1), tFactor)});
                }
            }
            if(dot(lightDir3, n3) <= 0.0)
            {
                float tFactor = effectiveRadius - sqrtf(Math::square(lightDir2));
                if(tFactor > 0.0f)
                {
                    ++noStencilBounds;
                    stencilRays.insert({i, Ray(shapePos2, -norm(lightDir2), tFactor)});
                    if(i < smallestKey) smallestKey = i;
                }
            }
        }
        else
        {
            float tFactor = effectiveRadius - sqrtf(Math::square(lightDir2));
            stencilRays.insert({i, Ray(shapePos2, -norm(lightDir2), tFactor)});
        }
    }
    if(noStencilBounds == 2)
    {
        using Math::modulo;
        using Math::norm;
        using Math::orthogonal;
        using Math::dot;
        using Math::cross;
        using Math::square;

        sf::Vector2f paraLine = norm(shape.getPosition()-lightPos);
        sf::Vector2f perpLine = orthogonal(paraLine, -1.0);
        float minCosine = 1e+15;
        float maxCosine = -minCosine;


        sf::VertexArray result(sf::LineStrip, stencilRays.size());

        int sRSize = stencilRays.size();
        Ray& firstRay = stencilRays[modulo(smallestKey, n)];
        Ray& lastRay = stencilRays[modulo(smallestKey+sRSize-1, n)];
        for(int j=0; j<sRSize; ++j)
        {
            Ray& temp = stencilRays[modulo(j+smallestKey, n)];

            sf::Vector2f rayEnd = temp.pos + effectiveRadius*temp.dir;
            result[j].position = temp.pos;

            float cosine = dot(norm(temp.pos - lightPos), lastRay.dir - firstRay.dir);
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

        float shapeSize = sqrtf(Math::square(result[sRSize - 1].position -
                                                result[1].position));
        umbralShader.setUniform("lightWidth", lightProperties.umbralRadius);
        umbralShader.setUniform("lightPos", sf::Glsl::Vec3(lightPos.x, lightPos.y, 0.0f));
        umbralShader.setUniform("rayLength", effectiveRadius/sqrtf(square(shape.getPosition()-lightPos)));

        shadowTexture.draw(result, &umbralShader);

        return result;
    }

    sf::VertexArray result(sf::Points, 1);
    return result;
}

void LightSource::calcEffectiveRadius(float attFactor)
{
    float Kc = lightProperties.constant;
    float Kl = lightProperties.linear;
    float Kq = lightProperties.quadratic;
    float Im = lightProperties.lightMax;

    effectiveRadius = ( -Kl + sqrt(Kl*Kl - 4*Kq*(Kc - Im/attFactor)) )/(2.0f*Kq);
}

Subject LightSource::renderSubject;

sf::Shader* LightSource::shadowShader = nullptr;

std::string LightSource::shadowShaderCode = \
    "uniform sampler2D umbralTexture;" \
    "void main()" \
    "{" \
    " vec4 umbralColor = texture2D(umbralTexture, gl_TexCoord[0].xy);" \
    " " \
    " " \
    " gl_FragColor = umbralColor;" \
    "}";
