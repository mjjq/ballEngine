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
    return false;
}

bool Renderer::textureIsLoaded(std::string textureName)
{
    if(loadedTextures.find(textureName) != loadedTextures.end())
        return true;

    return false;
}

void Renderer::redrawAll(sf::RenderWindow &window)
{
    sf::Sprite newSprite;
    for(int i=0; i<(int)renderObjects.size(); ++i)
    {
        if(renderObjects[i]->shader != nullptr && lights.size() > 0)
        {
            renderObjects[i]->shader->setParameter("lightColor",
                                                   lights[0]->color);
            renderObjects[i]->shader->setParameter("lightPosition",
                                                   lights[0]->position);
            float rotation = sfVectorMath::PI * renderObjects[i]->primTransformable->getRotation() / 180.0f;
            renderObjects[i]->shader->setParameter("rotCosine",
                                                   cos(rotation));
            renderObjects[i]->shader->setParameter("rotSine",
                                                   sin(rotation));
        }
        renderObjects[i]->shader->setParameter("normalMap",
                                loadedTextures[renderObjects[i]->normalID]);
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
                if(loadTexture(ren->diffuseID) || textureIsLoaded(ren->diffuseID))
                {
                    ren->primShape->setTexture(&loadedTextures[ren->diffuseID]);
                    ren->primShape->setFillColor(sf::Color::White);
                    ren->primShape->setOutlineThickness(0);
                }
                if(loadShader(ren->shaderID) || shaderIsLoaded(ren->shaderID))
                {
                    ren->shader = &loadedShaders[ren->shaderID];
                }
                loadTexture(ren->normalID);
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

    if(sf::Shader::isAvailable() && !shaderIsLoaded(shaderName))
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
