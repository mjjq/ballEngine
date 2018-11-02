#ifndef COLLDETECT_H
#define COLLDETECT_H

#include "classPhysicsObject.h"
#include "classBall.h"
#include "classAABB.h"
#include "classOBB.h"

class Collisions
{
    static sf::RenderWindow* debugWindow;
public:
    static void setDebugWindow(sf::RenderWindow &window);

    static float rayAABBIntersect(sf::Vector2f rayStart, sf::Vector2f rayDir,
                      sf::Rect<float > &rectAABB, float tmin,
                      float tmax, float epsilon = 1e-5);
    static float raySphereIntersect(sf::Vector2f rayOrigin, sf::Vector2f rayDir,
                     sf::Vector2f sphereCentre, float sphereRadius);

    static float timeToCollision(PhysicsObject* p1, PhysicsObject* p2);
    static float timeToCollBallBall(Ball *firstBall, Ball *secondBall);
    static float timeToCollBallAABB(Ball* origBall, AABB* origAABB);
    static float timeToCollAABBAABB(AABB* rect1, AABB* rect2);
    static float timeToCollBallOBB(Ball *ball, OBB* rect);

    static void resolveCollision(PhysicsObject* p1, PhysicsObject* p2);
    static void collisionBallBall(Ball* firstBall, Ball* secondBall);
    static void collisionBallAABB(Ball* origBall, AABB* origAABB);
    static void collisionAABBAABB(AABB* rect1, AABB* rect2);
    static void collisionBallOBB(Ball* ball, OBB* rect);

    static sf::Vector2f calcPenetVector(sf::Vector2f rayStart, sf::Vector2f rayNorm, Ball &ball);
    static sf::Vector2f calcPenetVector(Ball* ball1, Ball* ball2);
    static sf::Vector2f calcPenetVector(AABB &rect1, AABB &rect2);

    static sf::Vector2f calcContactNorm(AABB &rect1, AABB &rect2);

    static sf::Rect<float > minkowskiDiff(AABB &rect1, AABB &rect2);



};

#endif // COLLDETECT_H
