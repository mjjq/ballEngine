#ifndef CLASS_BALL_H
#define CLASS_BALL_H

#include <deque>
#include "integrators.h"
#include "classDynamicObject.h"

class Ball : public DynamicObject
{

    float radius;

public:
    Ball(float radius, float mass, sf::Vector2f initPos, sf::Vector2f initVel);
    ~Ball();

    static const ObjectType MY_TYPE = ObjectType::Ball;
    ObjectType type() const override;

    float getRadius();
    float getMinSize();

    void draw(sf::RenderWindow &_window);

    float getGPE(Ball &otherBall);

    float getDistance(Ball &otherBall);

    float getRelSpeed(Ball &otherBall);


};

#endif // CLASS_UNIVERSE_H
