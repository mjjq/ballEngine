#ifndef CLASS_BALL_H
#define CLASS_BALL_H

#include <deque>
#include "integrators.h"
#include "classPhysicsObject.h"

class Ball : public PhysicsObject
{
    float radius;

public:
    Ball(ObjectProperties init);
    ~Ball();

    static const ObjectType MY_TYPE = ObjectType::Ball;
    ObjectType type() const override;

    float getRadius();
    float getMinSize();
    sf::Vector2f getCoM();

    void draw(sf::RenderWindow &_window);

    std::vector<sf::Vertex > constructVerts() { return {{getPosition()}};}

    int getVertexCount() { return 1; }

    sf::Vertex farthestPointInDir(sf::Vector2f direction);

    sf::Rect<float > getBoundingBox();
};

#endif // CLASS_UNIVERSE_H
