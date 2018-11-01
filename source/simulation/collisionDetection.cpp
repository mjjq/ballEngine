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


#define CONCAT2(x,y) x##y
#define CONCAT(x,y) CONCAT2(x,y)

#define REGISTER_TCOLL_FUNCTION(o1,o2,fn) \
    const bool CONCAT(__reg_, __LINE__) = []() { \
        int o1type = static_cast<int>(o1::MY_TYPE); \
        int o2type = static_cast<int>(o2::MY_TYPE); \
        assert(o1type <= o2type); \
        assert(!TCollFunctionTable[o1type][o2type]); \
        TCollFunctionTable[o1type][o2type] = \
            [](PhysicsObject* p1, PhysicsObject* p2) { \
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
            [](PhysicsObject* p1, PhysicsObject* p2) { \
                    (*fn)(static_cast<o1*>(p1), static_cast<o2*>(p2)); \
            }; \
        return true; \
    }();

std::function<float(PhysicsObject*,PhysicsObject*)>
    TCollFunctionTable[(int)(ObjectType::_Count)][(int)(ObjectType::_Count)];
REGISTER_TCOLL_FUNCTION(Ball, Ball, &Collisions::timeToCollBallBall)
REGISTER_TCOLL_FUNCTION(Ball, AABB, &Collisions::timeToCollBallAABB)
REGISTER_TCOLL_FUNCTION(AABB, AABB, &Collisions::timeToCollAABBAABB)

std::function<void(PhysicsObject*,PhysicsObject*)>
    ResolveFunctionTable[(int)(ObjectType::_Count)][(int)(ObjectType::_Count)];
REGISTER_RESOLVE_FUNCTION(Ball, Ball, &Collisions::collisionBallBall)
REGISTER_RESOLVE_FUNCTION(Ball, AABB, &Collisions::collisionBallAABB)
REGISTER_RESOLVE_FUNCTION(AABB, AABB, &Collisions::collisionAABBAABB)


float Collisions::timeToCollision(PhysicsObject* p1, PhysicsObject* p2)
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

void Collisions::resolveCollision(PhysicsObject* p1, PhysicsObject* p2)
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
                                  sf::Rect<float > &rectAABB,
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
        if(r.x < rectAABB.left || r.x > rectAABB.left + rectAABB.width)
            return std::numeric_limits<float>::quiet_NaN();
    }
    else
    {
        float ood = 1.0f / v.x;
        float t1 = (rectAABB.left - r.x) * ood;
        float t2 = (rectAABB.left + rectAABB.width - r.x) * ood;

        if(t1>t2) std::swap(t1,t2);
        tmin = std::max(t1, tmin);
        tmax = std::min(t2, tmax);
        if(tmin > tmax) return std::numeric_limits<float>::quiet_NaN();
    }

    if(std::abs(v.y) < epsilon)
    {
        if(r.y < rectAABB.top || r.y > rectAABB.top + rectAABB.height)
            return std::numeric_limits<float>::quiet_NaN();
    }
    else
    {
        float ood = 1.0f / v.y;
        float t1 = (rectAABB.top - r.y) * ood;
        float t2 = (rectAABB.top + rectAABB.height - r.y) * ood;

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


float Collisions::timeToCollBallAABB(Ball* origBall, AABB* origAABB)
{
    sf::Vector2f v = origBall->getVelocity()-origAABB->getVelocity();
    sf::Vector2f r = origBall->getPosition();
    sf::Rect<float > minkSumAABB = origAABB->getGlobalBounds();
    minkSumAABB.left -= origBall->getRadius();
    minkSumAABB.top -= origBall->getRadius();
    minkSumAABB.width += 2.0f*origBall->getRadius();
    minkSumAABB.height += 2.0f*origBall->getRadius();
    //std::cout << "MinkAABB: " << AABB << "\n";

    float epsilon = 1e-5;
    float tmin = -1.0f;
    float tmax = 10000.0f;

    tmin = Collisions::rayAABBIntersect(r, v, minkSumAABB, tmin, tmax, epsilon);
    if(std::isnan(tmin))
        return tmin;

    sf::Vector2f intPoint = r+v*tmin;
    sf::Rect<float> origAABBRect = origAABB->getGlobalBounds();
    //std::cout << "MinkAABB: " << AABB << "\n";
    //std::cout << "origAABB" << origAABB << "\n";
    //std::cout << r << "\n\n";
    if(origAABBRect.contains(r.x,r.y))
    {
        return std::numeric_limits<float>::quiet_NaN();
    }

    bool intersectCorner = false;

    if(intPoint.x <= origAABBRect.left && intPoint.y <= origAABBRect.top)
    {
        tmin = raySphereIntersect(r,v, {origAABBRect.left, origAABBRect.top}, origBall->getRadius());
        intersectCorner = true;
    }

    else if(intPoint.x >= origAABBRect.left + origAABBRect.width && intPoint.y <= origAABBRect.top)
    {
        tmin = raySphereIntersect(r,v, {origAABBRect.left+origAABBRect.width, origAABBRect.top}, origBall->getRadius());
        intersectCorner = true;
    }

    else if(intPoint.x <= origAABBRect.left && intPoint.y >= origAABBRect.top + origAABBRect.height)
    {
        tmin = raySphereIntersect(r,v, {origAABBRect.left, origAABBRect.top+origAABBRect.height}, origBall->getRadius());
        intersectCorner = true;
    }

    else if(intPoint.x >= origAABBRect.left + origAABBRect.width && intPoint.y >= origAABBRect.top + origAABBRect.height)
    {
        tmin = raySphereIntersect(r,v, {origAABBRect.left+origAABBRect.width, origAABBRect.top+origAABBRect.height}, origBall->getRadius());
        intersectCorner = true;
    }

    else
        if(tmin < 0.0f)
        {
            if(minkSumAABB.contains(r.x, r.y) || intersectCorner)
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



float Collisions::timeToCollAABBAABB(AABB* rect1, AABB* rect2)
{
    sf::Vector2f v = rect1->getVelocity()-rect2->getVelocity();
    sf::Vector2f r = rect1->getPosition();

    sf::Rect<float > minkSumAABB = rect2->getGlobalBounds();

    sf::Rect<float > rect1Bounds = rect1->getGlobalBounds();
    minkSumAABB.left -= rect1Bounds.width;
    minkSumAABB.top -= rect1Bounds.height;
    minkSumAABB.width += rect1Bounds.width;
    minkSumAABB.height += rect1Bounds.height;


    sf::RectangleShape debug({minkSumAABB.width, minkSumAABB.height});
    debug.setPosition(minkSumAABB.left, minkSumAABB.top);
    //debug.setOutlineThickness(-1);
    debugWindow->draw(debug);


    //std::cout << "MinkAABB: " << AABB << "\n";

    float epsilon = 1e-5;
    float tmin = -1.0f;
    float tmax = 10000.0f;

    tmin = Collisions::rayAABBIntersect(r, v, minkSumAABB, tmin, tmax, epsilon);
    if(std::isnan(tmin))
        return tmin;

     sf::Vertex debugLine[] = {
        sf::Vertex(r),
        sf::Vertex(r+v*tmin)
    };
    debugLine[0].color = sf::Color::Red;
    debugLine[1].color = sf::Color::Red;
    debugWindow->draw(debugLine, 2, sf::Lines);

    sf::CircleShape comCircle{20.0f};
    comCircle.setOrigin(20.0f,20.0f);
    comCircle.setPosition(rect1Bounds.left,
                          rect1Bounds.top);
    comCircle.setFillColor(sf::Color::Red);
    debugWindow->draw(comCircle);


            std::cout << tmin << "\n";
    if(tmin < 0.0f)
    {
        if(minkSumAABB.contains(rect1Bounds.left,
                                rect1Bounds.top))
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

void Collisions::collisionBallAABB(Ball* origBall, AABB* origAABB)
{
    //std::cout << "Collisions\n";
    sf::Vector2f rBall = origBall->getPosition();
    sf::Vector2f v = origBall->getVelocity() - origAABB->getVelocity();
    sf::Vector2f rAABB = origAABB->getPosition();

    float redMassBall = origAABB->getMass()/(origAABB->getMass() + origBall->getMass());
    float redMassAABB = origBall->getMass()/(origAABB->getMass() + origBall->getMass());

    float coefRest = 0.7f;
    //std::cout << v << "\n";float penetDistance = distance - ball.getRadius();
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

    if((boolXMin || boolXMax) && !(boolYMin || boolYMax))
    {
        if(boolXMin)
            origBall->setPosition(origBall->getPosition() -
                             Collisions::calcPenetVector({rectBounds.left, rectBounds.top}, {-1.0f, 0.0f}, *origBall) );
        else if(boolXMax)
            origBall->setPosition(origBall->getPosition() -
                             Collisions::calcPenetVector({rectBounds.left+rectBounds.width, rectBounds.top}, {1.0f, 0.0f}, *origBall) );

        origBall->addSolvedVelocity({-coefRest*redMassBall*2.0f*v.x, 0.0f}, {-coefRest*redMassBall*2.0f*v.x, 0.0f});
        origAABB->addSolvedVelocity({coefRest*redMassAABB*2.0f*v.x, 0.0f}, {coefRest*redMassAABB*2.0f*v.x, 0.0f});
    }
    else if(!(boolXMin || boolXMax) && (boolYMin || boolYMax))
    {
        if(boolYMin)
            origBall->setPosition(origBall->getPosition() -
                Collisions::calcPenetVector({rectBounds.left, rectBounds.top}, {0.0f, -1.0f}, *origBall));
        else if(boolYMax)
            origBall->setPosition(origBall->getPosition() -
                Collisions::calcPenetVector({rectBounds.left, rectBounds.top+rectBounds.height}, {0.0f, 1.0f}, *origBall));

        origBall->addSolvedVelocity({0.0f, -coefRest*redMassBall*2.0f*v.y}, {0.0f, -coefRest*redMassBall*2.0f*v.y});
        origAABB->addSolvedVelocity({0.0f, coefRest*redMassAABB*2.0f*v.y}, {0.0f, coefRest*redMassAABB*2.0f*v.y});
    }
    else
    {
        sf::Vector2f ballPos = origBall->getPosition();
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
        penetVector = Collisions::calcPenetVector(cornerPos, contactNormal, *origBall);
        origBall->setPosition(ballPos - penetVector);

        sf::Vector2f dv = -coefRest*2.0f*sfVectorMath::dot(contactNormal, origBall->getVelocity())*contactNormal;
        origBall->addSolvedVelocity(redMassBall*dv, redMassBall*dv);
        origAABB->addSolvedVelocity(-redMassAABB*dv, -redMassAABB*dv);

        /*sf::Vertex line[2];
        line[0].position = cornerPos;
        line[1].position = ballPos;
        debugWindow->draw(line, 2, sf::Lines);*/
    }

}


void Collisions::collisionAABBAABB(AABB* rect1, AABB* rect2)
{

    sf::Vector2f r = rect1->getPosition();
    sf::Vector2f v = rect1->getVelocity() - rect2->getVelocity();

    float redMassRect1 = rect2->getMass()/(rect1->getMass() + rect2->getMass());
    float redMassRect2 = rect1->getMass()/(rect1->getMass() + rect2->getMass());

    float coefRest = 0.7f;
    //std::cout << v << "\n";float penetDistance = distance - ball.getRadius();
    sf::Rect<float> rect1Bounds = rect1->getGlobalBounds();
    sf::Rect<float> rect2Bounds = rect2->getGlobalBounds();

    bool boolXMin = false;
    bool boolXMax = false;
    bool boolYMin = false;
    bool boolYMax = false;

    /*if(rect1Bounds.left + rect1Bounds.width <= rect2Bounds.left)
        boolXMin = true;
    else if(rect1Bounds.left >= rect2Bounds.left + rect2Bounds.width)
        boolXMax = true;
    if(rect1Bounds.top + rect1Bounds.height <= rect2Bounds.top)
        boolYMin = true;
    else if(rect1Bounds.top >= rect2Bounds.top + rect2Bounds.height)
        boolYMax = true;*/

    if(rect1Bounds.left + rect1Bounds.width/2.0f <= rect2Bounds.left)
        boolXMin = true;
    else if(rect1Bounds.left >= rect2Bounds.left + rect2Bounds.width/2.0f)
        boolXMax = true;
    if(rect1Bounds.top + rect1Bounds.height/2.0f <= rect2Bounds.top)
        boolYMin = true;
    else if(rect1Bounds.top >= rect2Bounds.top + rect2Bounds.height/2.0f)
        boolYMax = true;

    std::cout << boolXMin << boolXMax << boolYMin << boolYMax << "\n";
    sf::Vector2f contactNormal = {0.0f, 0.0f};
    sf::Vector2f contactLinePos = {0.0f, 0.0f};
    if((boolXMin || boolXMax) && !(boolYMin || boolYMax))
    {
        if(boolXMin)
        {
            contactNormal = {-1.0f, 0.0f};
            contactLinePos = {rect2Bounds.left, rect2Bounds.top};
        }
        else if(boolXMax)
        {
            contactNormal = {1.0f, 0.0f};
            //contactLinePos = {rect2Bounds.left + rect2Bounds.width, rect2Bounds.top};
            contactLinePos = {rect2Bounds.left, rect2Bounds.top};
        }

        rect1->addSolvedVelocity({-coefRest*redMassRect1*2.0f*v.x, 0.0f}, {-coefRest*redMassRect1*2.0f*v.x, 0.0f});
        rect2->addSolvedVelocity({coefRest*redMassRect2*2.0f*v.x, 0.0f}, {coefRest*redMassRect2*2.0f*v.x, 0.0f});
    }
    else if(!(boolXMin || boolXMax) && (boolYMin || boolYMax))
    {
        if(boolYMin)
        {
            contactNormal = {0.0f, -1.0f};
            //contactLinePos = {rect2Bounds.left, rect2Bounds.top};
            contactLinePos = {rect2Bounds.left, rect2Bounds.top};
        }
        else if(boolYMax)
        {
            contactNormal = {0.0f, 1.0f};
            //contactLinePos = {rect2Bounds.left, rect2Bounds.top + rect2Bounds.height};
            contactLinePos = {rect2Bounds.left, rect2Bounds.top};
        }

        rect1->addSolvedVelocity({0.0f, -coefRest*redMassRect1*2.0f*v.y}, {0.0f, -coefRest*redMassRect1*2.0f*v.y});
        rect2->addSolvedVelocity({0.0f, coefRest*redMassRect2*2.0f*v.y}, {0.0f, coefRest*redMassRect2*2.0f*v.y});
    }
    if(sfVectorMath::square(contactNormal)>1e-15)
    {
        std::cout << "contact\n";
        sf::Vector2f penetVector = Collisions::calcPenetVector(contactLinePos, contactNormal, *rect1);
        rect1->setPosition(rect1->getPosition() - redMassRect1*penetVector);
        rect2->setPosition(rect2->getPosition() + redMassRect2*penetVector);
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

sf::Vector2f Collisions::calcPenetVector(sf::Vector2f rayStart, sf::Vector2f rayNorm, AABB &rect)
{
    float distance = sfVectorMath::dot( (rect.getPosition() - rayStart), rayNorm );
    sf::Rect<float > rectBounds = rect.getGlobalBounds();
    float penetDistance = 0.0f;
    sf::Vector2f penetVector = {0.0f, 0.0f};
    float epsilon = 1e-15;


    std::cout << distance << " Penetrated\n";
    if(std::abs(rayNorm.x) < epsilon && std::abs(rayNorm.y) > epsilon)
    {
        penetDistance = rectBounds.height - distance;
        penetVector = rayNorm*(penetDistance - 0.01f*rectBounds.height);
    }
    else
    {
        penetDistance = rectBounds.width - distance;
        penetVector = rayNorm*(penetDistance - 0.01f*rectBounds.width);
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
