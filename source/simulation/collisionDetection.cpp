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
#include "../../headers/classDynamicObject.h"


#define CONCAT2(x,y) x##y
#define CONCAT(x,y) CONCAT2(x,y)

#define REGISTER_TCOLL_FUNCTION(o1,o2,fn) \
    const bool CONCAT(__reg_, __LINE__) = []() { \
        int o1type = static_cast<int>(o1::MY_TYPE); \
        int o2type = static_cast<int>(o2::MY_TYPE); \
        assert(o1type <= o2type); \
        assert(!TCollFunctionTable[o1type][o2type]); \
        TCollFunctionTable[o1type][o2type] = \
            [](DynamicObject* p1, DynamicObject* p2) { \
                    return (*fn)(static_cast<o1*>(p1), static_cast<o2*>(p2)); \
            }; \
        return true; \
    }();

#define REGISTER_RESOLVE_FUNCTION(o1,o2,fn) \
    const bool CONCAT(__reg_, __LINE__) = []() { \
        int o1type = static_cast<int>(o1::MY_TYPE); \
        int o2type = static_cast<int>(o2::MY_TYPE); \
        assert(o1type <= o2type); \
        assert(!ResolveFunctionTable[o1type][o2type]); \
        ResolveFunctionTable[o1type][o2type] = \
            [](DynamicObject* p1, DynamicObject* p2) { \
                    (*fn)(static_cast<o1*>(p1), static_cast<o2*>(p2)); \
            }; \
        return true; \
    }();

std::function<float(DynamicObject*,DynamicObject*)>
    TCollFunctionTable[(int)(ObjectType::_Count)][(int)(ObjectType::_Count)];
REGISTER_TCOLL_FUNCTION(Ball, Ball, &Collisions::timeToCollBallBall)

std::function<void(DynamicObject*,DynamicObject*)>
    ResolveFunctionTable[(int)(ObjectType::_Count)][(int)(ObjectType::_Count)];
REGISTER_RESOLVE_FUNCTION(Ball, Ball, &Collisions::collisionBallBall)


float Collisions::timeToCollision(DynamicObject* p1, DynamicObject* p2)
{
    int p1type = static_cast<int>(p1->type());
    int p2type = static_cast<int>(p2->type());
    if(p1type > p2type) {
        std::swap(p1type, p2type);
        std::swap(p1, p2);
    }
    assert(TCollFunctionTable[p1type][p2type]);
    return TCollFunctionTable[p1type][p2type](p1, p2);
}

void Collisions::resolveCollision(DynamicObject* p1, DynamicObject* p2)
{
    int p1type = static_cast<int>(p1->type());
    int p2type = static_cast<int>(p2->type());
    if(p1type > p2type) {
        std::swap(p1type, p2type);
        std::swap(p1, p2);
    }
    assert(ResolveFunctionTable[p1type][p2type]);
    ResolveFunctionTable[p1type][p2type](p1, p2);
}

sf::RenderWindow* Collisions::debugWindow = nullptr;
void Collisions::setDebugWindow(sf::RenderWindow &window)
{
    debugWindow = &window;
    std::cout << debugWindow << "\n";
}


float Collisions::rayAABBIntersect(sf::Vector2f rayStart,
                                  sf::Vector2f rayDir,
                                  sf::Rect<float> &AABB,
                                  float tmin, float tmax, float epsilon)
{
    sf::Vector2f r = rayStart;
    sf::Vector2f v = rayDir;

    /*if(AABB.contains(r.x,r.y))
    {
        //colliderArray.printMatrix(); std::cout << "\n";
        //staticCollArray.printMatrix(); std::cout << "\n";
        return std::numeric_limits<float>::quiet_NaN();
    }*/

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
    //if(tmin < 0.0f || tmax < 0.0f)
    //    tmin = 1e+12;
    //std::cout << tmin << "\n";
    return tmin;
}

float Collisions::raySphereIntersect(sf::Vector2f rayOrigin, sf::Vector2f rayDir,
                                     sf::Vector2f sphereCentre, float sphereRadius)
{
    float a = sfVectorMath::square(rayDir);
    float b = 2.0f*sfVectorMath::dot(rayDir, (rayOrigin-sphereCentre) );
    float c = sfVectorMath::square( rayOrigin-sphereCentre )
                                                - sphereRadius*sphereRadius;
    float discriminant = b*b - 4.0f*a*c;
    if(discriminant < 0)
        return std::numeric_limits<float>::quiet_NaN();

    //std::cout << rayOrigin << "\n";
    //std::cout << rayDir << "\n";
    //std::cout << "SC: " << sphereCentre << "\n";

    float root1 = -(b + pow(discriminant,0.5f))/(2.0f*a);
    float root2 = -(b - pow(discriminant,0.5f))/(2.0f*a);

    /*std::cout << rayDir << "\n";
    std::cout << root1 << " root1\n";
    std::cout << root2 << " root2\n\n";*/

    //std::cout << root1 << " : " << root2 << "\n\n";
    if(root1 < 0 && root2 < 0)
        return std::numeric_limits<float>::quiet_NaN();
    if(!(root1 < 0) != !(root2 < 0))
        return 0.0f;
    return (root2<root1)?root2:root1;
}



/**
    Calculate the time to collision with another ball.

    @param &otherBall Reference to the other ball.

    @return Time to collision.
*/
float Collisions::timeToCollBallBall(Ball *firstBall, Ball *secondBall)
{
    using namespace sfVectorMath;

    sf::Vector2f relPos = firstBall->getPosition() - secondBall->getPosition();
    float radSum = firstBall->getRadius() + secondBall->getRadius();

    sf::Vector2f relVel = firstBall->getVelocity() - secondBall->getVelocity();

    float relSpeed = square(relVel);
    if(relSpeed < 1e-10)
        return std::numeric_limits<float>::quiet_NaN();
    float projVec = dot(relPos, relVel);

    float discriminant = pow(projVec, 2) -
                         relSpeed*(square(relPos) - pow(radSum, 2));

    if(discriminant < 0)
        return std::numeric_limits<float>::quiet_NaN();

    float root1 = -(projVec + pow(discriminant,0.5))/relSpeed;
    float root2 = -(projVec - pow(discriminant,0.5))/relSpeed;

    if(root1 < 0 && root2 < 0)
        return std::numeric_limits<float>::quiet_NaN();
    else if(!(root1 < 0) != !(root2 < 0))
        return 0.0f;
    return (root2<root1)?root2:root1;
}


float Collisions::timeToCollBallAABB(Ball &ball, sf::RectangleShape &origAARect)
{
    sf::Vector2f v = ball.getVelocity();
    sf::Vector2f r = ball.getPosition();
    sf::Rect<float> AABB = origAARect.getGlobalBounds();
    AABB.left -= ball.getRadius();
    AABB.top -= ball.getRadius();
    AABB.width += 2.0f*ball.getRadius();
    AABB.height += 2.0f*ball.getRadius();
    //std::cout << "MinkAABB: " << AABB << "\n";

    float epsilon = 1e-5;
    float tmin = -1.0f;
    float tmax = 10000.0f;

    tmin = Collisions::rayAABBIntersect(r, v, AABB, tmin, tmax, epsilon);
    if(std::isnan(tmin))
        return tmin;

    sf::Vector2f intPoint = r+v*tmin;
    sf::Rect<float> origAABB = origAARect.getGlobalBounds();
    //std::cout << "MinkAABB: " << AABB << "\n";
    //std::cout << "origAABB" << origAABB << "\n";
    //std::cout << r << "\n\n";
    if(origAABB.contains(r.x,r.y))
    {
        return std::numeric_limits<float>::quiet_NaN();
    }

    bool intersectCorner = false;

    if(intPoint.x <= origAABB.left && intPoint.y <= origAABB.top)
    {
        tmin = raySphereIntersect(r,v, {origAABB.left, origAABB.top}, ball.getRadius());
        intersectCorner = true;
    }

    else if(intPoint.x >= origAABB.left + origAABB.width && intPoint.y <= origAABB.top)
    {
        tmin = raySphereIntersect(r,v, {origAABB.left+origAABB.width, origAABB.top}, ball.getRadius());
        intersectCorner = true;
    }

    else if(intPoint.x <= origAABB.left && intPoint.y >= origAABB.top + origAABB.height)
    {
        tmin = raySphereIntersect(r,v, {origAABB.left, origAABB.top+origAABB.height}, ball.getRadius());
        intersectCorner = true;
    }

    else if(intPoint.x >= origAABB.left + origAABB.width && intPoint.y >= origAABB.top + origAABB.height)
    {
        tmin = raySphereIntersect(r,v, {origAABB.left+origAABB.width, origAABB.top+origAABB.height}, ball.getRadius());
        intersectCorner = true;
    }

    else
        if(tmin < 0.0f)
        {
            if(AABB.contains(r.x, r.y) || intersectCorner)
            {
                return 0.0f;
            }
            else
            {
                return std::numeric_limits<float>::quiet_NaN();
            }
        }
    return tmin;
}


void Collisions::collisionBallBall(Ball* firstBall, Ball* secondBall)
{
    using namespace sfVectorMath;

    float coefRest = 0.9f;
    //sf::Vector2f relPos = firstBall->getPosition() - secondBall->getPosition();
    sf::Vector2f rhat = norm(firstBall->getPosition() - secondBall->getPosition());

    sf::Vector2f v1 = firstBall->getVelocity();
    sf::Vector2f v2 = secondBall->getVelocity();
    float m1 = firstBall->getMass();
    float m2 = secondBall->getMass();

    sf::Vector2f penetVector = Collisions::calcPenetVector(firstBall, secondBall);
    sf::Vector2f penetVector1 = penetVector*m2/(m1+m2);
    sf::Vector2f penetVector2 = penetVector*m1/(m1+m2);
    firstBall->setPosition(firstBall->getPosition() - penetVector1);
    secondBall->setPosition(secondBall->getPosition() + penetVector2);

    firstBall->setVelocity(v1 - coefRest*rhat*dot(v1-v2,rhat)*(2*m2)/(m1+m2));
    secondBall->setVelocity(v2 + coefRest*rhat*dot(v1-v2,rhat)*(2*m1)/(m1+m2));

    firstBall->incTimesCollided();
    secondBall->incTimesCollided();
}

//void Collisions::ballCollision(Ball &ball, )

void Collisions::ballCollision(Ball &ball, sf::RectangleShape &rect)
{
    //std::cout << "Collisions\n";
    sf::Vector2f r = ball.getPosition();
    sf::Vector2f v = ball.getVelocity();

    float coefRest = 0.7f;
    //std::cout << v << "\n";float penetDistance = distance - ball.getRadius();
    sf::Rect<float> rectBounds = rect.getGlobalBounds();

    bool boolXMin = false;
    bool boolXMax = false;
    bool boolYMin = false;
    bool boolYMax = false;

    if(r.x <= rectBounds.left)
        boolXMin = true;
    else if(r.x >= rectBounds.left + rectBounds.width)
        boolXMax = true;
    if(r.y <= rectBounds.top)
        boolYMin = true;
    else if(r.y >= rectBounds.top + rectBounds.height)
        boolYMax = true;

    if((boolXMin || boolXMax) && !(boolYMin || boolYMax))
    {
        if(boolXMin)
            ball.setPosition(ball.getPosition() -
                             Collisions::calcPenetVector({rectBounds.left, rectBounds.top}, {-1.0f, 0.0f}, ball) );
        else if(boolXMax)
            ball.setPosition(ball.getPosition() -
                             Collisions::calcPenetVector({rectBounds.left+rectBounds.width, rectBounds.top}, {1.0f, 0.0f}, ball) );

        ball.addSolvedVelocity({-coefRest*2.0f*v.x, 0.0f}, {-coefRest*2.0f*v.x, 0.0f});
    }
    else if(!(boolXMin || boolXMax) && (boolYMin || boolYMax))
    {
        if(boolYMin)
            ball.setPosition(ball.getPosition() -
                Collisions::calcPenetVector({rectBounds.left, rectBounds.top}, {0.0f, -1.0f}, ball));
        else if(boolYMax)
            ball.setPosition(ball.getPosition() -
                Collisions::calcPenetVector({rectBounds.left, rectBounds.top+rectBounds.height}, {0.0f, 1.0f}, ball));

        ball.addSolvedVelocity({0.0f, -coefRest*2.0f*v.y}, {0.0f, -coefRest*2.0f*v.y});
    }
    else
    {
        sf::Vector2f ballPos = ball.getPosition();
        sf::Vector2f contactNormal = {0.0f, 0.0f};
        sf::Vector2f cornerPos = {0.0f, 0.0f};
        sf::Vector2f penetVector = {0.0f,0.0f};

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

        contactNormal = sfVectorMath::norm(ballPos - cornerPos);
        penetVector = Collisions::calcPenetVector(cornerPos, contactNormal, ball);
        ball.setPosition(ballPos - penetVector);

        sf::Vector2f dv = -coefRest*2.0f*sfVectorMath::dot(contactNormal, ball.getVelocity())*contactNormal;
        ball.addSolvedVelocity(dv, dv);

        /*sf::Vertex line[2];
        line[0].position = cornerPos;
        line[1].position = ballPos;
        debugWindow->draw(line, 2, sf::Lines);*/
    }

}


sf::Vector2f Collisions::calcPenetVector(sf::Vector2f rayStart, sf::Vector2f rayNorm, Ball &ball)
{
    float distance = sfVectorMath::dot( (ball.getPosition() - rayStart), rayNorm );
    float penetDistance = -1.0f*std::abs(distance - ball.getRadius());
    //if(penetDistance >= 0.0f)
    //    return sf::Vector2f{0.0f,0.0f};
    return rayNorm*(penetDistance-0.01f*ball.getRadius());
}

sf::Vector2f Collisions::calcPenetVector(Ball* ball1, Ball* ball2)
{
    sf::Vector2f separationVec = ball1->getPosition() - ball2->getPosition();
    float distance = sqrt( sfVectorMath::square(separationVec) );
    float penetDistance = -1.0f*std::abs( distance - (ball1->getRadius() + ball2->getRadius()) );

    return (separationVec/distance)*(penetDistance -0.001f*(ball1->getRadius() + ball2->getRadius()));
}
