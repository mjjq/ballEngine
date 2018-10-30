#ifndef COLLDETECT_H
#define COLLDETECT_H

#include "classBall.h"

class Collisions
{
    static sf::RenderWindow* debugWindow;
public:
    static void setDebugWindow(sf::RenderWindow &window);

    static float rayAABBIntersect(sf::Vector2f rayStart, sf::Vector2f rayDir,
                      sf::Rect<float> &AABB, float tmin,
                      float tmax, float epsilon = 1e-5);
    static float raySphereIntersect(sf::Vector2f rayOrigin, sf::Vector2f rayDir,
                     sf::Vector2f sphereCentre, float sphereRadius);
    static float timeToCollision(Ball &firstBall, Ball &secondBall);
    static float timeToCollision(Ball &ball, sf::RectangleShape &origAARect);

    static void ballCollision(Ball &firstBall, Ball &secondBall);
    static void ballCollision(Ball &ball, sf::RectangleShape &rect);

    static sf::Vector2f calcPenetVector(sf::Vector2f rayStart, sf::Vector2f rayNorm, Ball &ball);
    static sf::Vector2f calcPenetVector(Ball &ball1, Ball &ball2);

};

#endif // COLLDETECT_H
