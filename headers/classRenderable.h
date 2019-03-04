#ifndef CLASS_RENDERABLE_H
#define CLASS_RENDERABLE_H

#include "SFML/Graphics.hpp"
#include "Observer.h"

class Renderable : public Entity
{
public:
    Renderable(std::string _texID,
               std::vector<sf::Drawable* > _primitives = {});
    static Subject renderSubject;
    static const std::string NULL_TEXTURE;
    std::string textureID;
    std::vector<sf::Drawable* > primitives;
};

#endif // CLASS_RENDERABLE_H
