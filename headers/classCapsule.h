#ifndef CLASS_CAPSULE_H
#define CLASS_CAPSULE_H

#include <deque>
#include "integrators.h"
#include "classPhysicsObject.h"

class Capsule : public PhysicsObject
{
    std::vector<sf::Vertex > vertices = {};
    sf::Rect<float> boundingOBB;
    sf::Vector2f centrePosition = {0.0f, 0.0f};
    float radius;
    float length;

    void genBoundingOBB();

public:
    Capsule(ObjectProperties init);
    ~Capsule();

    sf::Rect<float> getGlobalBounds();

    static const ObjectType MY_TYPE = ObjectType::Capsule;
    ObjectType type() const override;

    float getMinSize();
    sf::Vector2f getCoM();
    sf::Rect<float > getBoundingBox();
    std::vector<sf::Vertex > constructVerts();
    std::vector<sf::Vertex > constructLocalVerts();
    float getRotAngle();

    float getRadius();

    void draw(sf::RenderWindow &_window);

    sf::Vertex farthestPointInDir(sf::Vector2f direction);

};

#endif // CLASS_UNIVERSE_H

