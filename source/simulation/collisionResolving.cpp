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
REGISTER_RESOLVE_FUNCTION(OBB, OBB,   &Collisions::collisionOBBOBB)


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

    ClippedPoints cp;
    cp.push_back(firstBall->getPosition() - rhat*firstBall->getRadius());

    Collisions::applyImpulse(firstBall,
                             secondBall,
                             rhat,
                             cp);
    //firstBall->setVelocity(v1 - coefRest*rhat*dot(v1-v2,rhat)*(2*m2)/(m1+m2));
    //secondBall->setVelocity(v2 + coefRest*rhat*dot(v1-v2,rhat)*(2*m1)/(m1+m2));

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
    sf::Vector2f contactNormal = {0.0f, 0.0f};
    sf::Vector2f cornerPos = {0.0f, 0.0f};

    if((boolXMin || boolXMax) && !(boolYMin || boolYMax))
    {
        if(boolXMin)
        {
            cornerPos = {rectBounds.left, rectBounds.top};
            contactNormal = {-1.0f, 0.0f};
        }
        else if(boolXMax)
        {
            cornerPos = {rectBounds.left+rectBounds.width, rectBounds.top};
            contactNormal = {1.0f, 0.0f};
        }
    }
    else if(!(boolXMin || boolXMax) && (boolYMin || boolYMax))
    {
        if(boolYMin)
        {
            cornerPos = {rectBounds.left, rectBounds.top};
            contactNormal = {0.0f, -1.0f};
        }
        else if(boolYMax)
        {
            cornerPos = {rectBounds.left, rectBounds.top+rectBounds.height};
            contactNormal = {0.0f, 1.0f};
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

        contactNormal = sfVectorMath::norm(rBall - cornerPos);
    }

    penetVector = Collisions::calcPenetVector(cornerPos, contactNormal, *origBall);

    origBall->setPosition(rBall - redMassBall*penetVector);
    origAABB->setPosition(rAABB + redMassAABB*penetVector);

    //Collisions::applyImpulse(origBall, origAABB, contactNormal);
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

void Collisions::collisionOBBOBB(OBB* rect1, OBB* rect2)
{
    std::vector<sf::Vertex > rect1Vert = rect1->constructVerts();
    std::vector<sf::Vertex > rect2Vert = rect2->constructVerts();

    /*sf::VertexArray quad1(sf::TriangleStrip, 4);
    for(int i=0; i<3; ++i)
        quad1[i] = rect1Vert[i];

    sf::VertexArray quad2(sf::LineStrip, 4);
    for(int i=0; i<3; ++i)
        quad2[i] = rect2Vert[i];

    debugWindow->draw(quad1);
    debugWindow->draw(quad2);*/

    sf::Vector2f penetVector = Collisions::sepAxisTest(rect1Vert, rect2Vert).second;
    sf::Vector2f contactNorm = sfVectorMath::norm(penetVector);

    float redMass = rect1->getMass()*rect2->getMass()/(rect1->getMass() + rect2->getMass());
    penetVector += 0.1f*contactNorm;

    ClippedPoints cp = Collisions::getContactPoints(rect1Vert, rect2Vert, contactNorm);

    //sf::CircleShape circ1{2.0f};
    //circ1.setPosition(*cp.begin());
    //sf::CircleShape circ2{2.0f};
    //circ2.setPosition(*cp.end());

    //debugWindow->draw(circ1);
    //debugWindow->draw(circ2);

    rect1->setPosition(rect1->getPosition() + redMass*penetVector/rect1->getMass());
    rect2->setPosition(rect2->getPosition() - redMass*penetVector/rect2->getMass());


    Collisions::applyImpulse(rect1, rect2, contactNorm, cp);
}

void Collisions::applyImpulse(PhysicsObject *obj1,
                              PhysicsObject *obj2,
                              sf::Vector2f contactNorm,
                              ClippedPoints &collisionPoints)
{
    sf::Vector2f relVel = obj2->getVelocity() - obj1->getVelocity();
    for(sf::Vector2f &collPoint : collisionPoints)
    {
        sf::Vector2f rA = collPoint - obj1->getCoM();
        sf::Vector2f rB = collPoint - obj2->getCoM();
        relVel += Collisions::orthogonal(rA, obj1->getRotRate()) -
                    Collisions::orthogonal(rB, obj2->getRotRate());
    }
    relVel = relVel/static_cast<float>(collisionPoints.size());

    float coefRest = 0.2f;
    float mu = 0.3f;

    //std::cout << contactNorm << "\n";
    sf::Vector2f contactTangent = relVel - sfVectorMath::dot(relVel, contactNorm)*contactNorm;
    if(sfVectorMath::square(contactTangent) > 0.0f)
        contactTangent = sfVectorMath::norm(contactTangent);

    float j = 0.0f;
    float jt = 0.0f;

    sf::Vector2f resVectorA = {0.0f, 0.0f};
    sf::Vector2f resVectorB = {0.0f, 0.0f};

    float IA = obj1->getMomentInertia();
    float IB = obj2->getMomentInertia();

    for(sf::Vector2f &collPoint : collisionPoints)
    {
        sf::Vector2f rA = collPoint - obj1->getCoM();
        sf::Vector2f rB = collPoint - obj2->getCoM();
        resVectorA += rA;
        resVectorB += rB;
        float cTermA = pow( sfVectorMath::cross(rA, contactNorm), 2 )/IA;
        float cTermB = pow( sfVectorMath::cross(rB, contactNorm), 2 )/IB;

        float denom = (1.0f/obj1->getMass() + 1.0f/obj2->getMass() + cTermA + cTermB);

        j += (1+coefRest)*sfVectorMath::dot(relVel, contactNorm)/denom;

        cTermA = pow( sfVectorMath::cross(rA, contactTangent), 2 )/IA;
        cTermB = pow( sfVectorMath::cross(rB, contactTangent), 2 )/IB;

        denom = (1.0f/obj1->getMass() + 1.0f/obj2->getMass() + cTermA + cTermB);

        jt += sfVectorMath::dot(relVel, contactTangent)/denom;

    }
    j = j/collisionPoints.size();
    jt = jt/collisionPoints.size();
    resVectorA = resVectorA/static_cast<float>(collisionPoints.size());
    resVectorB = resVectorB/static_cast<float>(collisionPoints.size());

    //std::cout << resVectorA << "\n";

    sf::Vector2f impulse = j*contactNorm;
    sf::Vector2f frictionImpulse;

    float dwA = 0.0f;
    float dwB = 0.0f;

    if(std::abs(jt) < j*mu)
    {
        frictionImpulse = jt * contactTangent;

        dwA = sfVectorMath::cross(resVectorA, contactTangent) * jt / IA;
        dwB = sfVectorMath::cross(resVectorB, contactTangent) * jt / IB;
    }
    else
    {
        frictionImpulse = j * contactTangent * mu;

        dwA = sfVectorMath::cross(resVectorA, contactTangent) * j / IA;
        dwB = sfVectorMath::cross(resVectorB, contactTangent) * j / IB;
        std::cout << frictionImpulse << "dynamic\n";
    }

    dwA += sfVectorMath::cross(resVectorA, contactNorm) * j / IA;
    dwB += sfVectorMath::cross(resVectorB, contactNorm) * j / IB;

sf::CircleShape circ1{2.5f};
    circ1.setPosition(obj1->getCoM());
    circ1.setOrigin({1.25f, 1.25f});
    circ1.setFillColor(sf::Color::Red);
    debugWindow->draw(circ1);

    obj1->addSolvedVelocity((impulse + frictionImpulse)/obj1->getMass(),
                             (impulse + frictionImpulse)/obj1->getMass());
    obj2->addSolvedVelocity(-(impulse + frictionImpulse)/obj2->getMass(),
                             -(impulse + frictionImpulse)/obj2->getMass());

    std::cout << contactNorm << "\n";
    //std::cout << obj1->getMomentInertia() << "\n";
    obj1->addRotRate(dwA);
    obj2->addRotRate(-dwB);
}
