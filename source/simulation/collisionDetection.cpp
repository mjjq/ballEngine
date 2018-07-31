#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "../../headers/collisionDetection.h"
#include "../../headers/sfVectorMath.h"

float Collisions::rayAABBIntersect(sf::Vector2f rayStart,
                                  sf::Vector2f rayDir,
                                  sf::Rect<float> &AABB,
                                  float tmin, float tmax, float epsilon)
{
    sf::Vector2f r = rayStart;
    sf::Vector2f v = rayDir;

    if(AABB.contains(r.x,r.y))
    {
        //colliderArray.printMatrix(); std::cout << "\n";
        //staticCollArray.printMatrix(); std::cout << "\n";
        return std::numeric_limits<float>::quiet_NaN();
    }

    if(std::abs(v.x) < epsilon)
    {
        if(r.x < AABB.left || r.x > AABB.left + AABB.width)
            return std::numeric_limits<float>::quiet_NaN();
    }
    else
    {
        float ood = 1.0f / v.x;
        float t1 = (AABB.left - r.x) * ood;
        float t2 = (AABB.left + AABB.width - r.x) * ood;

        if(t1>t2) std::swap(t1,t2);
        tmin = std::max(t1, tmin);
        tmax = std::min(t2, tmax);
        if(tmin > tmax) return std::numeric_limits<float>::quiet_NaN();
    }

    if(std::abs(v.y) < epsilon)
    {
        if(r.y < AABB.top || r.y > AABB.top + AABB.height)
            return std::numeric_limits<float>::quiet_NaN();
    }
    else
    {
        float ood = 1.0f / v.y;
        float t1 = (AABB.top - r.y) * ood;
        float t2 = (AABB.top + AABB.height - r.y) * ood;

        if(t1>t2) std::swap(t1,t2);
        tmin = std::max(t1, tmin);
        tmax = std::min(t2, tmax);
        if(tmin > tmax) return std::numeric_limits<float>::quiet_NaN();
    }
    return tmin;
}


/**
    Calculate the time to collision with another ball.

    @param &otherBall Reference to the other ball.

    @return Time to collision.
*/
float Collisions::timeToCollision(Ball &firstBall, Ball &secondBall)
{
    using namespace sfVectorMath;

    sf::Vector2f relPos = firstBall.getPosition() - secondBall.getPosition();
    float radSum = firstBall.getRadius() + secondBall.getRadius();

    if(dot(relPos,relPos) <= pow(radSum, 2))
        return std::numeric_limits<float>::quiet_NaN();

    sf::Vector2f relVel = firstBall.getVelocity() - secondBall.getVelocity();
    float discriminant = pow(dot(relPos,relVel), 2) -
                         dot(relVel,relVel)*(dot(relPos,relPos) - pow(radSum, 2));

    if(discriminant < 0)
        return std::numeric_limits<float>::quiet_NaN();

    float root1 = -(dot(relPos,relVel) + pow(discriminant,0.5))/dot(relVel,relVel);
    float root2 = -(dot(relPos,relVel) - pow(discriminant,0.5))/dot(relVel,relVel);

    if(root1 < 0 && root2 < 0)
        return std::numeric_limits<float>::quiet_NaN();
    else if(root1 < 0 && root2 > 0)
        return root2;
    else if(root2 < 0 && root1 > 0)
        return root1;
    return (root2<root1)?root2:root1;
}


float Collisions::timeToCollision(Ball &ball, sf::RectangleShape &origAARect)
{
    sf::Vector2f v = ball.getVelocity();
    sf::Vector2f r = ball.getPosition();
    sf::Rect<float> AABB = origAARect.getGlobalBounds();
    AABB.left -= ball.getRadius();
    AABB.top -= ball.getRadius();
    AABB.width += 2.0f*ball.getRadius();
    AABB.height += 2.0f*ball.getRadius();
    //std::cout << AABB << "\n";

    float epsilon = 1e-5;
    float tmin = 0.0f;
    float tmax = 10000.0f;

    tmin = Collisions::rayAABBIntersect(r, v, AABB, tmin, tmax, epsilon);
    if(tmin = std::numeric_limits<float>::quiet_NaN())
        return tmin;

    sf::Vector2f intPoint = r+v*tmin;
    sf::Rect origAABB = origAARect.getGlobalBounds();

    if(intPoint.x < origAABB.left && intPoint.y < origAABB.top)
        tmin = std::min(tmin, raySphereIntersect())
    else if(intPoint.x > origAABB.left + origAABB.width && intPoint.y < origAABB.top)

    else if(intPoint.x < origAABB.left && intPoint.y > origAABB.top + origAABB.height)

    else if(intPoint.x > origAABB.left + origAABB.width && intPoint.y > origAABB.top + origAABB.height)

    return tmin;
}


void Collisions::ballCollision(Ball &firstBall, Ball &secondBall)
{
    using namespace sfVectorMath;

    //sf::Vector2f relPos = firstBall.getPosition() - secondBall.getPosition();
    sf::Vector2f rhat = norm(firstBall.getPosition() - secondBall.getPosition());

    sf::Vector2f v1 = firstBall.getVelocity();
    sf::Vector2f v2 = secondBall.getVelocity();
    float m1 = firstBall.getMass();
    float m2 = secondBall.getMass();

    firstBall.setVelocity(v1 - rhat*dot(v1-v2,rhat)*(2*m2)/(m1+m2));
    secondBall.setVelocity(v2 + rhat*dot(v1-v2,rhat)*(2*m1)/(m1+m2));
}

void Collisions::ballCollision(Ball &ball, sf::RectangleShape &rect)
{
    sf::Vector2f r = ball.getPosition();
    sf::Vector2f v = ball.getVelocity();
    sf::Rect<float> rectBounds = rect.getGlobalBounds();
    if(r.x <= rectBounds.left || r.x >= rectBounds.left + rectBounds.width)
        ball.setVelocity({-v.x, v.y});
    else if(r.y <= rectBounds.top || r.y >= rectBounds.top + rectBounds.height)
        ball.setVelocity({v.x, -v.y});
    else
        ball.setVelocity({-v.x, -v.y});
}
