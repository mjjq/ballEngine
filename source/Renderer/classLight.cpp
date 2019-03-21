#include "classLight.h"
#include "sfVectorMath.h"
#include <iostream>
#include <cmath>
#include "stringConversion.h"

LightSource::LightSource(LightProperties properties) :
    lightProperties{properties}, position{properties.position}
{
    calcEffectiveRadius(5.0f/256.0f);
    std::cout << effectiveRadius << " radius\n";
    if(brightShader == nullptr)
    {
        brightShader = new sf::Shader();
        brightShader->loadFromMemory(brightShaderCode, sf::Shader::Fragment);
    }
    if(brightShader2 == nullptr)
    {
        brightShader2 = new sf::Shader();
        brightShader2->loadFromMemory(brightShaderCode2, sf::Shader::Fragment);
    }
    if(shadowShader == nullptr)
    {
        shadowShader = new sf::Shader();
        shadowShader->loadFromMemory(shadowShaderCode, sf::Shader::Fragment);
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
    std::vector<Ray > stencilRays;
    sf::Vector2f lightPos = {position.x, position.y};

    sf::Transform trans = shape.getTransform();

    int n=shape.getPointCount();
    for(int i=0; i<(int)n; ++i)
    {
        using sfVectorMath::orthogonal;
        using sfVectorMath::dot;
        using sfVectorMath::norm;
        using sfVectorMath::modulo;

        sf::Vector2f shapePos0 = trans.transformPoint(shape.getPoint(modulo((i-2),n)));
        sf::Vector2f shapePos1 = trans.transformPoint(shape.getPoint(modulo((i-1),n)));
        sf::Vector2f shapePos2 = trans.transformPoint(shape.getPoint(modulo((i),n)));
        sf::Vector2f shapePos3 = trans.transformPoint(shape.getPoint(modulo((i+1),n)));

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
                float tFactor = effectiveRadius - sqrtf(sfVectorMath::square(lightDir1));
                if(tFactor > 0.0f)
                    stencilRays.push_back(Ray(shapePos1, -norm(lightDir1), tFactor));
            }
            if(dot(lightDir3, n3) <= 0.0)
            {
                float tFactor = effectiveRadius - sqrtf(sfVectorMath::square(lightDir2));
                if(tFactor > 0.0f)
                    stencilRays.push_back(Ray(shapePos2, -norm(lightDir2), tFactor));
            }
        }

        if(stencilRays.size() == 2)
        {
            brightShader->setUniform("existingTexture", shadowTexture.getTexture());
            shadowTexture.draw(shape, brightShader);

            sf::VertexArray result(sf::TriangleStrip, 4);
            result[0].position = stencilRays[0].pos;
            result[1].position = stencilRays[1].pos;
            result[2].position = stencilRays[0].pos +
                                 stencilRays[0].maxT*stencilRays[0].dir;
            result[3].position = stencilRays[1].pos +
                                 stencilRays[1].maxT*stencilRays[1].dir;
            result[0].color = sf::Color(0,0,0,255);
            result[1].color = sf::Color(0,0,0,255);
            result[2].color = sf::Color(0,0,0,255);
            result[3].color = sf::Color(0,0,0,255);

            shadowShader->setUniform("existingTexture", shadowTexture.getTexture());
            shadowTexture.draw(result, shadowShader);

            brightShader2->setUniform("existingTexture", shadowTexture.getTexture());
            shadowTexture.draw(shape, brightShader2);

            return result;
        }

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

sf::Shader* LightSource::brightShader = nullptr;

std::string LightSource::brightShaderCode = \
    "uniform sampler2D existingTexture;" \
    "void main()" \
    "{" \
    " ivec2 texSize = textureSize(existingTexture, 0);" \
    " vec4 tex = texture2D(existingTexture, vec2(gl_FragCoord.x/float(texSize.x), gl_FragCoord.y/float(texSize.y))); " \
    " " \
    " " \
    " " \
    "   gl_FragColor = vec4(1.0*tex.g, 0.0, 0.0, 1.0);" \
    "}";

sf::Shader* LightSource::brightShader2 = nullptr;

std::string LightSource::brightShaderCode2 = \
    "uniform sampler2D existingTexture;" \
    "void main()" \
    "{" \
    " ivec2 texSize = textureSize(existingTexture, 0);" \
    " vec4 tex = texture2D(existingTexture, vec2(gl_FragCoord.x/float(texSize.x), gl_FragCoord.y/float(texSize.y))); " \
    " " \
    " " \
    " " \
    "   gl_FragColor = vec4(0.0, 0.0, 1.0*tex.r, 1.0);" \
    "}";

sf::Shader* LightSource::shadowShader = nullptr;

std::string LightSource::shadowShaderCode = \
    "uniform sampler2D existingTexture;" \
    "void main()" \
    "{" \
    " ivec2 texSize = textureSize(existingTexture, 0);" \
    " vec4 tex = texture2D(existingTexture, vec2(gl_FragCoord.x/float(texSize.x), gl_FragCoord.y/float(texSize.y))); " \
    " " \
    " " \
    " if(tex.g > 0.0)" \
    "   gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);" \
    " else if(tex.b > 0.0)" \
    "   gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);"    \
    "}";