#ifndef COLLDETECT_H
#define COLLDETECT_H

#include "classBall.h"

namespace Collisions
{
    float rayAABBIntersect(sf::Vector2f rayStart, sf::Vector2f rayDir,
                      sf::Rect<float> &AABB, float tmin,
                      float tmax, float epsilon = 1e-5);
    float raySphereIntersect(sf::Vector2f rayOrigin, sf::Vector2f rayDir,
                     sf::Vector2f sphereCentre, float sphereRadius);
    float timeToCollision(Ball &firstBall, Ball &secondBall);
    float timeToCollision(Ball &ball, sf::RectangleShape &origAARect);

    void ballCollision(Ball &firstBall, Ball &secondBall);
    void ballCollision(Ball &ball, sf::RectangleShape &rect);

    sf::Vector2f calcPenetVector(sf::Vector2f rayStart, sf::Vector2f rayNorm, Ball &ball);
    sf::Vector2f calcPenetVector(Ball &ball1, Ball &ball2);

}

#endif // COLLDETECT_H
