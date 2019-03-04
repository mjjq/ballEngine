#include "classRenderable.h"

Renderable::Renderable(std::string _texID,
               std::vector<sf::Drawable* > _primitives) : Entity()
{
    textureID = _texID;
    primitives = _primitives;
    renderSubject.notify(*this, Event(EventType::New_Renderable));
}

Subject Renderable::renderSubject;

const std::string Renderable::NULL_TEXTURE = "";
