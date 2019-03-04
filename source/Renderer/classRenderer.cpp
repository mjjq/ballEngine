#include "classRenderer.h"

Renderer::Renderer()
{
    Renderable::renderSubject.addObserver(this);
}

void Renderer::redrawAll(sf::RenderWindow &window)
{
    sf::Sprite newSprite;
    for(int i=0; i<(int)renderObjects.size(); ++i)
    {
        for(int j=0; j<(int)renderObjects[i]->primitives.size(); ++j)
            window.draw(*renderObjects[i]->primitives[j]);

        std::string &texID = renderObjects[i]->textureID;

        if(texID != Renderable::NULL_TEXTURE)
            newSprite.setTexture(loadedTextures[texID]);


        window.draw(newSprite);
    }
}

void Renderer::onNotify(Entity& entity, Event event)
{
    if(event.type == EventType::New_Renderable)
        renderObjects.push_back((Renderable*)&entity);
}
