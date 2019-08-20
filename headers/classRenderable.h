#ifndef CLASS_RENDERABLE_H
#define CLASS_RENDERABLE_H

#include "SFML/Graphics.hpp"
#include "Observer.h"
#include "baseObject.h"

class Renderable : public Component
{
    float zPosition = 0.0f;

    void generateDrawables(ObjectProperties objProps);

public:
    static const std::string NULL_ID;

    Material material;

    sf::Drawable* primDrawable;
    sf::Transformable* primTransformable;
    sf::Shape* primShape;

    sf::Shader* shader = nullptr;

    static Subject renderSubject;

    Renderable(std::string _texID,
               std::vector<sf::Drawable* > _primitives = {});
    Renderable(ObjectProperties objProps);
    ~Renderable();
    //std::vector<sf::Drawable* > primitives;

    void updatePosition(sf::Vector2f position);
    void updateOrientation(float angle);
    void setScale(sf::Vector2f const & scale);

    float getZPosition() { return zPosition; }
};

#endif // CLASS_RENDERABLE_H
