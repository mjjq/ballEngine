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

std::function<void(PhysicsObject*,PhysicsObject*)>
    ResolveFunctionTable[(int)(ObjectType::_Count)][(int)(ObjectType::_Count)];
REGISTER_RESOLVE_FUNCTION(Ball, Ball, &Collisions::collisionBallBall)
REGISTER_RESOLVE_FUNCTION(Ball, AABB, &Collisions::collisionBallAABB)
REGISTER_RESOLVE_FUNCTION(AABB, AABB, &Collisions::collisionAABBAABB)
REGISTER_RESOLVE_FUNCTION(Ball, OBB,  &Collisions::collisionBallOBB)

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
    sf::Vector2f rAABB = origAABB->getPosition();
    sf::Vector2f v = origBall->getVelocity() - origAABB->getVelocity();

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

    sf::Vector2f penetVector = {0.0f, 0.0f};

    if((boolXMin || boolXMax) && !(boolYMin || boolYMax))
    {
        if(boolXMin)
            penetVector = Collisions::calcPenetVector({rectBounds.left, rectBounds.top}, {-1.0f, 0.0f}, *origBall);
        else if(boolXMax)
            penetVector = Collisions::calcPenetVector({rectBounds.left+rectBounds.width, rectBounds.top}, {1.0f, 0.0f}, *origBall);

        origBall->addSolvedVelocity({-coefRest*redMassBall*2.0f*v.x, 0.0f}, {-coefRest*redMassBall*2.0f*v.x, 0.0f});
        origAABB->addSolvedVelocity({coefRest*redMassAABB*2.0f*v.x, 0.0f}, {coefRest*redMassAABB*2.0f*v.x, 0.0f});
    }
    else if(!(boolXMin || boolXMax) && (boolYMin || boolYMax))
    {
        if(boolYMin)
            penetVector = Collisions::calcPenetVector({rectBounds.left, rectBounds.top}, {0.0f, -1.0f}, *origBall);
        else if(boolYMax)
            penetVector = Collisions::calcPenetVector({rectBounds.left, rectBounds.top+rectBounds.height}, {0.0f, 1.0f}, *origBall);

        origBall->addSolvedVelocity({0.0f, -coefRest*redMassBall*2.0f*v.y}, {0.0f, -coefRest*redMassBall*2.0f*v.y});
        origAABB->addSolvedVelocity({0.0f, coefRest*redMassAABB*2.0f*v.y}, {0.0f, coefRest*redMassAABB*2.0f*v.y});
    }
    else
    {
        sf::Vector2f contactNormal = {0.0f, 0.0f};
        sf::Vector2f cornerPos = {0.0f, 0.0f};

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

        contactNormal = sfVectorMath::norm(rBall - cornerPos);
        penetVector = Collisions::calcPenetVector(cornerPos, contactNormal, *origBall);
        //penetVector += 0.01f*contactNormal;

        sf::Vector2f dv = -coefRest*2.0f*sfVectorMath::dot(contactNormal, v)*contactNormal;
        origBall->addSolvedVelocity(redMassBall*dv, redMassBall*dv);
        origAABB->addSolvedVelocity(-redMassAABB*dv, -redMassAABB*dv);
    }

    origBall->setPosition(rBall - redMassBall*penetVector);
    origAABB->setPosition(rAABB + redMassAABB*penetVector);
}


void Collisions::collisionAABBAABB(AABB* rect1, AABB* rect2)
{

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

    sf::Vector2f rCent = {rect1Bounds.left + rect1Bounds.width/2.0f,
                         rect1Bounds.top  + rect1Bounds.height/2.0f};
    if(rCent.x <= rect2Bounds.left)
        boolXMin = true;
    else if(rCent.x >= rect2Bounds.left + rect2Bounds.width)
        boolXMax = true;
    if(rCent.y <= rect2Bounds.top)
        boolYMin = true;
    else if(rCent.y >= rect2Bounds.top + rect2Bounds.height)
        boolYMax = true;

    //std::cout << boolXMin << boolXMax << boolYMin << boolYMax << "\n";
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
            contactLinePos = {rect2Bounds.left + rect2Bounds.width, rect2Bounds.top};
            //contactLinePos = {rect2Bounds.left, rect2Bounds.top};
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
            contactLinePos = {rect2Bounds.left, rect2Bounds.top + rect2Bounds.height};
            //contactLinePos = {rect2Bounds.left, rect2Bounds.top};
        }

        rect1->addSolvedVelocity({0.0f, -coefRest*redMassRect1*2.0f*v.y}, {0.0f, -coefRest*redMassRect1*2.0f*v.y});
        rect2->addSolvedVelocity({0.0f, coefRest*redMassRect2*2.0f*v.y}, {0.0f, coefRest*redMassRect2*2.0f*v.y});
    }
    else
    {
        contactNormal =  Collisions::calcContactNorm(*rect1, *rect2);

        sf::Vector2f newVel{v.x*std::abs(contactNormal.x), v.y*std::abs(contactNormal.y)};
        rect1->addSolvedVelocity(-newVel*coefRest*redMassRect1*2.0f, -newVel*coefRest*redMassRect1*2.0f);
        rect2->addSolvedVelocity(newVel*coefRest*redMassRect2*2.0f, newVel*coefRest*redMassRect2*2.0f);
    }

    if(sfVectorMath::square(contactNormal)>1e-15)
    {
        sf::Vector2f penetVector = Collisions::calcPenetVector(*rect1, *rect2);

        penetVector -= 0.1f*contactNormal;
        rect1->setPosition(rect1->getPosition() - redMassRect1*penetVector);
        rect2->setPosition(rect2->getPosition() + redMassRect2*penetVector);
    }
}

void Collisions::collisionBallOBB(Ball* ball, OBB* rect)
{

        float rotAngle = rect->getRotAngle();
        sf::Rect<float > rectBounds = rect->getGlobalBounds();

        sf::Vector2f ballVelocity = sfVectorMath::rotate(ball->getVelocity(), -rotAngle);
        sf::Vector2f rectVelocity = sfVectorMath::rotate(rect->getVelocity(), -rotAngle);
        sf::Vector2f ballPosition = sfVectorMath::rotate(ball->getPosition(), -rotAngle);
        sf::Vector2f rectPosition = sfVectorMath::rotate(rect->getPosition(), -rotAngle)
                                  - sf::Vector2f{rectBounds.width/2.0f, rectBounds.height/2.0f};


        AABB obbInFrame{{rectBounds.width, rectBounds.height}, rect->getMass(),
                        rectPosition,
                        rectVelocity};
        sf::Vector2f relVelocity = ball->getVelocity() - rect->getVelocity();
        sf::Vector2f sepVector = ball->getPosition() - rect->getPosition();
        relVelocity = sfVectorMath::rotate(relVelocity, -rotAngle);
        sepVector = sfVectorMath::rotate(sepVector, -rotAngle);

        Ball ballInFrame{ball->getRadius(), ball->getMass(), ballPosition, ballVelocity};

        Collisions::collisionBallAABB(&ballInFrame, &obbInFrame);

        sf::Vector2f newBallVelocity = sfVectorMath::rotate(ballInFrame.getVelocity(), rotAngle);
        sf::Vector2f newRectVelocity = sfVectorMath::rotate(obbInFrame.getVelocity(), rotAngle);
        sf::Vector2f newBallPosition = sfVectorMath::rotate(ballInFrame.getPosition(), rotAngle);
        sf::Vector2f newRectPosition = sfVectorMath::rotate(obbInFrame.getPosition()
                                     + sf::Vector2f{rectBounds.width/2.0f, rectBounds.height/2.0f}, rotAngle);

        ball->setVelocity(newBallVelocity);
        ball->setPosition(newBallPosition);
        rect->setVelocity(newRectVelocity);
        rect->setPosition(newRectPosition);
}
