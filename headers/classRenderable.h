#ifndef CLASS_RENDERABLE_H
#define CLASS_RENDERABLE_H

#include "SFML/Graphics.hpp"
#include "Observer.h"
#include "baseObject.h"


class Renderable : public Entity
{
    void generateDrawables(ObjectProperties objProps);

public:
    static const std::string NULL_TEXTURE;
    std::string textureID;
    sf::Drawable* primDrawable;
    sf::Transformable* primTransformable;
    sf::Shape* primShape;

    static Subject renderSubject;

    Renderable(std::string _texID,
               std::vector<sf::Drawable* > _primitives = {});
    Renderable(ObjectProperties objProps);
    ~Renderable();
    //std::vector<sf::Drawable* > primitives;

    void updatePosition(sf::Vector2f position);
    void updateOrientation(float angle);
};

#endif // CLASS_RENDERABLE_H
