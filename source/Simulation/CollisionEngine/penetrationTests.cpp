#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <cassert>

#include "collisionDetection.h"
#include "sfVectorMath.h"
#include "stringConversion.h"


sf::Vector2f Collisions::calcPenetVector(Ball* ball1, Ball* ball2)
{
    sf::Vector2f separationVec = ball2->getPosition() - ball1->getPosition();
    float distance = sqrtf( sfVectorMath::square(separationVec) );
    float penetDistance =  distance - (ball1->getRadius() + ball2->getRadius());
    //std::cout << penetDistance << " pd\n";

    return (separationVec/distance)*(penetDistance);
}

sf::Vector2f Collisions::calcPenetVector(sf::Vector2f rayStart, sf::Vector2f rayNorm, Ball &ball)
{
    float distance = sfVectorMath::dot( (rayStart - ball.getPosition()), rayNorm );
    float penetDistance = distance - ball.getRadius();
    //if(penetDistance >= 0.0f)
    //    return sf::Vector2f{0.0f,0.0f};
    return rayNorm*(penetDistance);
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


std::pair<sf::Vector2f, sf::Vector2f> Collisions::getContactNormal(Ball *origBall, AABB *origAABB)
{
    sf::Vector2f rBall = origBall->getPosition();
    sf::Rect<float> rectBounds = origAABB->getGlobalBounds();

    bool boolXMin = false;
    bool boolXMax = false;
    bool boolYMin = false;
    bool boolYMax = false;

    if(rBall.x <= rectBounds.left)
        boolXMin = true;
    else if(rBall.x >= rectBounds.left + rectBounds.width)
        boolXMax = true;
    if(rBall.y <= rectBounds.top)
        boolYMin = true;
    else if(rBall.y >= rectBounds.top + rectBounds.height)
        boolYMax = true;

    sf::Vector2f contactNormal = {0.0f, 0.0f};
    sf::Vector2f cornerPos = {0.0f, 0.0f};

    if((boolXMin || boolXMax) && !(boolYMin || boolYMax))
    {
        if(boolXMin)
        {
            cornerPos = {rectBounds.left, rectBounds.top};
            contactNormal = {1.0f, 0.0f};
        }
        else if(boolXMax)
        {
            cornerPos = {rectBounds.left+rectBounds.width, rectBounds.top};
            contactNormal = {-1.0f, 0.0f};
        }
    }
    else if(!(boolXMin || boolXMax) && (boolYMin || boolYMax))
    {
        if(boolYMin)
        {
            cornerPos = {rectBounds.left, rectBounds.top};
            contactNormal = {0.0f, 1.0f};
        }
        else if(boolYMax)
        {
            cornerPos = {rectBounds.left, rectBounds.top+rectBounds.height};
            contactNormal = {0.0f, -1.0f};
        }
    }
    else
    {
        if(boolXMin && boolYMin)
        {
            cornerPos = sf::Vector2f{rectBounds.left, rectBounds.top};
        }
        else if(boolXMax && boolYMin)
        {
            cornerPos = sf::Vector2f{rectBounds.left + rectBounds.width, rectBounds.top};
        }
        else if(boolXMin && boolYMax)
        {
            cornerPos = sf::Vector2f{rectBounds.left, rectBounds.top + rectBounds.height};

        }
        else if(boolXMax && boolYMax)
        {
            cornerPos = sf::Vector2f{rectBounds.left + rectBounds.width, rectBounds.top + rectBounds.height};
        }

        contactNormal = sfVectorMath::norm(cornerPos-rBall);
    }

    return std::make_pair(contactNormal, cornerPos);
}

std::pair<sf::Vector2f, sf::Vector2f> Collisions::getContactNormal(Ball* ball, Polygon* poly)
{
    std::vector<sf::Vertex > verts = poly->constructVerts();
    sf::Vector2f relPos     = {0.0f, 0.0f};
    sf::Vector2f relPos2    = {0.0f, 0.0f};
    sf::Vector2f edgeNorm   = {0.0f, 0.0f};
    sf::Vector2f edgeNormVert  = {0.0f, 0.0f};
    sf::Vector2f cornerPos  = {0.0f, 0.0f};
    sf::Vector2f cornerPosVert  = {0.0f, 0.0f};

    //float ballRad = ball->getRadius();

    float distance = 1e+15;
    //float minVertDistSq = 1e+15;

    bool touchCorner = true;

    //int theVertex = 0;

    for(unsigned int i=0; i<verts.size(); ++i)
    {
        sf::Vector2f tangent = verts[(i+1)%verts.size()].position - verts[i].position;
        sf::Vector2f norm = sfVectorMath::norm( sfVectorMath::orthogonal(tangent, 1.0f) );
        relPos = ball->getPosition() - verts[i].position;
        relPos2 = ball->getPosition() - verts[(i+1)%verts.size()].position;

        float vertDistSq = sfVectorMath::square(relPos);
        float normDist = sfVectorMath::dot(relPos, norm);
        float tanDist = sfVectorMath::dot(relPos, sfVectorMath::norm(tangent));
        float segDistSq = sfVectorMath::square(tangent);

        //theVertex = i;

        if(std::abs(normDist) <= distance)
        {
            if(tanDist >= 0.0f && tanDist*tanDist <= segDistSq)
            {
                distance = std::abs(normDist);
                edgeNorm = norm;
                cornerPos = verts[i].position;
                touchCorner = false;

            }
        }
        if(vertDistSq <= distance*distance)
        {
            distance = sqrtf(vertDistSq);
            cornerPosVert = verts[i].position;
            edgeNormVert = sfVectorMath::norm(ball->getPosition() -
                                      cornerPosVert);
            touchCorner = true;
        }

    }
    if(!touchCorner)
        return std::make_pair(sfVectorMath::norm(edgeNorm), cornerPos);

    return std::make_pair(sfVectorMath::norm(edgeNormVert), cornerPosVert);
}
