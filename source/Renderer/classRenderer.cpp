#include "classRenderer.h"
#include <iostream>

Renderer::Renderer()
{
    Renderable::renderSubject.addObserver(this);
}

bool Renderer::loadTexture(std::string textureName)
{
    if(loadedTextures.find(textureName) == loadedTextures.end() &&
       textureName != Renderable::NULL_TEXTURE)
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
        //for(int j=0; j<(int)renderObjects[i]->primitives.size(); ++j)
            window.draw(*renderObjects[i]->primDrawable);

        /*std::string &texID = renderObjects[i]->textureID;

        if(texID != Renderable::NULL_TEXTURE)
            newSprite.setTexture(loadedTextures[texID]);


        window.draw(newSprite);*/
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
                if(loadTexture(ren->textureID) || textureIsLoaded(ren->textureID))
                {
                    ren->primShape->setTexture(&loadedTextures[ren->textureID]);
                    ren->primShape->setFillColor(sf::Color::White);
                    ren->primShape->setOutlineThickness(0);
                }
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
        default:
            break;
    }
}
