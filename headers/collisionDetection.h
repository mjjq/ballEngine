#ifndef COLLDETECT_H
#define COLLDETECT_H

#include "classBall.h"

namespace Collisions
{
    float rayAABBIntersect(sf::Vector2f rayStart, sf::Vector2f rayDir,
                      sf::Rect<float> &AABB, float tmin,
                      float tmax, float epsilon = 1e-5);
    float timeToCollision(Ball &firstBall, Ball &secondBall);
    float timeToCollision(Ball &ball, sf::RectangleShape &origAARect);

    void ballCollision(Ball &firstBall, Ball &secondBall);
    void ballCollision(Ball &ball, sf::RectangleShape &rect);
}

#endif // COLLDETECT_H
