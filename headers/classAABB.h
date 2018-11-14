#ifndef CLASS_AABB_H
#define CLASS_AABB_H

#include <deque>
#include "integrators.h"
#include "classPhysicsObject.h"

class AABB : public PhysicsObject
{
    sf::Vector2f size;
    sf::Vector2f centreOfMass;

public:
    AABB(sf::Vector2f _size, float mass, sf::Vector2f initPos, sf::Vector2f initVel);
    ~AABB();

    sf::Rect<float> getGlobalBounds();

    static const ObjectType MY_TYPE = ObjectType::AABB;
    ObjectType type() const override;

    float getMinSize();
    //sf::Vector2f getCoM();

    void draw(sf::RenderWindow &_window);

    std::vector<sf::Vertex > constructVerts();

};

#endif // CLASS_UNIVERSE_H

