#include "classRenderer.h"
#include <iostream>
#include <cmath>
#include "sfVectorMath.h"

Renderer::Renderer()
{
    Renderable::renderSubject.addObserver(this);
    LightSource::renderSubject.addObserver(this);
}

bool Renderer::loadTexture(std::string textureName)
{
    if(loadedTextures.find(textureName) == loadedTextures.end() &&
       textureName != Renderable::NULL_ID)
    {
        sf::Texture texture;
        if(texture.loadFromFile(TEXTURE_PATH + textureName))
        {
            loadedTextures.insert({textureName, texture});
            return true;
        }
    }

    std::cout << "failed to load texture: " << textureName << "\n";
    return false;
}

bool Renderer::textureIsLoaded(std::string textureName)
{
    if(loadedTextures.find(textureName) != loadedTextures.end() &&
       textureName != Renderable::NULL_ID)
        return true;

    std::cout << "texture isn't loaded\n";
    return false;
}

void Renderer::redrawAll(sf::RenderWindow &window)
{
    sf::Sprite newSprite;
    for(int i=0; i<(int)renderObjects.size(); ++i)
    {
        if(renderObjects[i]->shader != nullptr)
        {
            sf::Shader* shader = renderObjects[i]->shader;
            for(int j=0; j<10; ++j)
            {
                std::string lightVal = "lights[" + std::to_string(j) + "]";
                if(j < lights.size())
                {
                    shader->setParameter("light.color", lights[j]->lightProperties.color);
                    shader->setParameter(lightVal + ".color", lights[j]->lightProperties.color);
                    shader->setParameter(lightVal + ".position", lights[j]->position);
                    shader->setParameter(lightVal + ".constant", lights[j]->lightProperties.constant);
                    shader->setParameter(lightVal + ".linear", lights[j]->lightProperties.linear);
                    shader->setParameter(lightVal + ".quadratic", lights[j]->lightProperties.quadratic);
                }
                else
                {
                    shader->setParameter(lightVal + ".color", sf::Vector3f(0.0,0.0,0.0));
                    shader->setParameter(lightVal + ".position", sf::Vector3f(0.0,0.0,0.0));
                    shader->setParameter(lightVal + ".constant", 1.0f);
                    shader->setParameter(lightVal + ".linear", 0.0f);
                    shader->setParameter(lightVal + ".quadratic", 0.0f);
                }
            }

            float rotation = sfVectorMath::PI * renderObjects[i]->primTransformable->getRotation() / 180.0f;
            shader->setParameter("rotCosine", cos(rotation));
            shader->setParameter("rotSine", sin(rotation));

            shader->setParameter("material.diffuseMap", loadedTextures[renderObjects[i]->material.diffuseID]);
            shader->setParameter("material.normalMap", loadedTextures[renderObjects[i]->material.normalID]);
            shader->setParameter("material.emissionMap", loadedTextures[renderObjects[i]->material.emissionID]);
            shader->setParameter("material.diffuseStrength", renderObjects[i]->material.diffuseStrength);
            shader->setParameter("material.ambientStrength", renderObjects[i]->material.ambientStrength);
            shader->setParameter("material.specularStrength", renderObjects[i]->material.specularStrength);
            shader->setParameter("material.emissionStrength", renderObjects[i]->material.emissionStrength);
            shader->setParameter("material.shininess", renderObjects[i]->material.shininess);
        }

        window.draw(*renderObjects[i]->primDrawable, renderObjects[i]->shader);
    }
}

void Renderer::onNotify(Entity& entity, Event event)
{
    switch(event.type)
    {
        case(EventType::New_Renderable):
        {
            Renderable* ren = (Renderable*)&entity;
            if(ren->primShape != nullptr)
                if(loadTexture(ren->material.diffuseID) || textureIsLoaded(ren->material.diffuseID))
                {
                    std::cout << "assigned texture\n";
                    ren->primShape->setTexture(&loadedTextures[ren->material.diffuseID]);
                    ren->primShape->setFillColor(sf::Color::White);
                    ren->primShape->setOutlineThickness(0);
                }
                if(loadShader(ren->material.shaderID) || shaderIsLoaded(ren->material.shaderID))
                {
                    ren->shader = &loadedShaders[ren->material.shaderID];
                }
                loadTexture(ren->material.normalID);
                loadTexture(ren->material.emissionID);
            renderObjects.push_back(ren);
            break;
        }
        case(EventType::Delete_Renderable):
        {
            Renderable* obj = (Renderable*)&entity;
            for(int i=0; i<renderObjects.size(); ++i)
                if(renderObjects[i] == obj)
                    renderObjects.erase(renderObjects.begin() + i);
            break;
        }
        case(EventType::New_LightSrc):
        {
            LightSource* lSrc = (LightSource*)&entity;
            lights.push_back(lSrc);
            break;
        }
        case(EventType::Delete_LightSrc):
        {
            LightSource* obj = (LightSource*)&entity;
            for(int i=0; i<lights.size(); ++i)
                if(lights[i] == obj)
                    lights.erase(lights.begin() + i);
            break;
        }
        default:
            break;
    }
}

bool Renderer::loadShader(std::string shaderName)
{
    std::string filePath = SHADER_PATH + shaderName;

    if(sf::Shader::isAvailable() &&
       !shaderIsLoaded(shaderName) &&
       shaderName != Renderable::NULL_ID)
    {
        bool isFragment = false;
        bool isVert = false;

        if(shaderName.find(FRAGMENT_EXTENSION) != std::string::npos)
            isFragment = true;
        else if(shaderName.find(VERT_EXTENSION) != std::string::npos)
            isVert = true;

        if(isFragment)
        {

            if(loadedShaders[shaderName].loadFromFile(filePath, sf::Shader::Fragment))
                return true;
        }
        else if(isVert)
        {
            if(loadedShaders[shaderName].loadFromFile(filePath, sf::Shader::Vertex))
                return true;
        }
        else
        {
            std::string vertName = filePath + ".vert";
            std::string fragName = filePath + ".frag";

            if(loadedShaders[shaderName].loadFromFile(vertName, fragName))
                return true;
        }
    }
    std::cout << "failed to load shaders: " << shaderName << "\n";
    return false;
}

bool Renderer::shaderIsLoaded(std::string shaderName)
{
    if(loadedShaders.find(shaderName) != loadedShaders.end())
        return true;

    return false;
}
