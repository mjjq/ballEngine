#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <cassert>

#include "../../headers/collisionDetection.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"

sf::Vector2f Collisions::calcPenetVector(sf::Vector2f rayStart, sf::Vector2f rayNorm, Ball &ball)
{
    float distance = sfVectorMath::dot( (ball.getPosition() - rayStart), rayNorm );
    float penetDistance = -1.0f*std::abs(distance - ball.getRadius());
    //if(penetDistance >= 0.0f)
    //    return sf::Vector2f{0.0f,0.0f};
    return rayNorm*(penetDistance-0.01f);
}


sf::Vector2f Collisions::calcPenetVector(AABB &rect1, AABB &rect2)
{
    sf::Rect<float > minkDiff = Collisions::minkowskiDiff(rect1, rect2);

    sf::Vector2f penetVector = {0.0f, 0.0f};

    if(minkDiff.contains(0.0f, 0.0f))
    {
        float minVal = 1e+20;

        if(std::abs(minkDiff.left) < minVal)
        {
            minVal = std::abs(minkDiff.left);
            penetVector = {minkDiff.left, 0.0f};
        }
        if(std::abs(minkDiff.left + minkDiff.width) < minVal)
        {
            minVal = std::abs(minkDiff.left + minkDiff.width);
            penetVector = {minkDiff.left + minkDiff.width, 0.0f};
        }
        if(std::abs(minkDiff.top) < minVal)
        {
            minVal = std::abs(minkDiff.top);
            penetVector = {0.0f, minkDiff.top};
        }
        if(std::abs(minkDiff.top + minkDiff.height) < minVal)
        {
            minVal = std::abs(minkDiff.top + minkDiff.height);
            penetVector = {0.0f, minkDiff.top + minkDiff.height};
        }
    }

    return penetVector;
}


sf::Vector2f Collisions::calcContactNorm(AABB &rect1, AABB &rect2)
{
    sf::Rect<float > minkDiff = Collisions::minkowskiDiff(rect1, rect2);

    sf::Vector2f penetVector = {0.0f, 0.0f};

    //if(!minkDiff.contains(0.0f, 0.0f))
    {
        float minVal = 1e+20;

        if(std::abs(minkDiff.left) < minVal)
        {
            minVal = std::abs(minkDiff.left);
            penetVector = {1.0f, 0.0f};
        }
        if(std::abs(minkDiff.left + minkDiff.width) < minVal)
        {
            minVal = std::abs(minkDiff.left + minkDiff.width);
            penetVector = {-1.0f, 0.0f};
        }
        if(std::abs(minkDiff.top) < minVal)
        {
            minVal = std::abs(minkDiff.top);
            penetVector = {0.0f, 1.0f};
        }
        if(std::abs(minkDiff.top + minkDiff.height) < minVal)
        {
            minVal = std::abs(minkDiff.top + minkDiff.height);
            penetVector = {0.0f, -1.0f};
        }
    }

    return penetVector;
}


sf::Vector2f Collisions::calcPenetVector(Ball* ball1, Ball* ball2)
{
    sf::Vector2f separationVec = ball1->getPosition() - ball2->getPosition();
    float distance = sqrt( sfVectorMath::square(separationVec) );
    float penetDistance = -1.0f*std::abs( distance - (ball1->getRadius() + ball2->getRadius()) );

    return (separationVec/distance)*(penetDistance -0.001f*(ball1->getRadius() + ball2->getRadius()));
}
