#ifndef CLASS_RENDERABLE_H
#define CLASS_RENDERABLE_H

#include "SFML/Graphics.hpp"
#include "Observer.h"
#include "baseObject.h"

class Renderable : public Component, public sf::Transformable
{
    float zPosition = 0.0f;

    void generateDrawables(ObjectProperties objProps);

public:
    static const std::string NULL_ID;

    Material material;

    sf::Shader* shader = nullptr;
    sf::Texture* diffuseMap = nullptr;
    sf::Texture* normalMap = nullptr;
    sf::Texture* emissionMap = nullptr;

    sf::VertexArray verts;

    static Subject renderSubject;

    Renderable(ObjectProperties objProps);
    ~Renderable();

    float getZPosition() { return zPosition; }
};

#endif // CLASS_RENDERABLE_H
