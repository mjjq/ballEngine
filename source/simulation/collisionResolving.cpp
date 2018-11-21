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
#include "../../headers/constraintSolver.h"

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
                    return (*fn)(static_cast<o1*>(p1), static_cast<o2*>(p2)); \
            }; \
        return true; \
    }();

std::function<std::vector<Contact>(PhysicsObject*,PhysicsObject*)>
    ResolveFunctionTable[(int)(ObjectType::_Count)][(int)(ObjectType::_Count)];
REGISTER_RESOLVE_FUNCTION(Ball, Ball, &Collisions::collisionBallBall)
REGISTER_RESOLVE_FUNCTION(Ball, AABB, &Collisions::collisionBallAABB)
REGISTER_RESOLVE_FUNCTION(AABB, AABB, &Collisions::collisionAABBAABB)
REGISTER_RESOLVE_FUNCTION(Ball, OBB,  &Collisions::collisionBallOBB)
REGISTER_RESOLVE_FUNCTION(OBB, OBB,   &Collisions::collisionOBBOBB)
REGISTER_RESOLVE_FUNCTION(OBB, Polygon,  &Collisions::collisionOBBPoly)
REGISTER_RESOLVE_FUNCTION(Ball, Polygon,   &Collisions::collisionBallPoly)
REGISTER_RESOLVE_FUNCTION(Polygon, Polygon,   &Collisions::collisionPolyPoly)


std::vector<Contact> Collisions::resolveCollision(PhysicsObject* p1, PhysicsObject* p2)
{
    int p1type = static_cast<int>(p1->type());
    int p2type = static_cast<int>(p2->type());
    bool swapState = false;
    if(p1type > p2type) {
        std::swap(p1type, p2type);
        std::swap(p1, p2);
        swapState = true;
    }
    assert(ResolveFunctionTable[p1type][p2type]);

    std::vector<Contact> contResult;

    contResult = ResolveFunctionTable[p1type][p2type](p1, p2);

    if(swapState)
    {
        for(Contact &temp : contResult)
        {
            temp.normal = -temp.normal;
        }
    }

    return contResult;
}



std::vector<Contact> Collisions::collisionBallBall(Ball* firstBall, Ball* secondBall)
{
    using namespace sfVectorMath;

    //float coefRest = 0.9f;
    //sf::Vector2f relPos = firstBall->getPosition() - secondBall->getPosition();
    sf::Vector2f rhat = norm(secondBall->getPosition() - firstBall->getPosition());

    //sf::Vector2f v1 = firstBall->getVelocity();
    //sf::Vector2f v2 = secondBall->getVelocity();
    float m1 = firstBall->getMass();
    float m2 = secondBall->getMass();

    sf::Vector2f penetVector = Collisions::calcPenetVector(firstBall, secondBall);

    ClippedPoints cp;
    std::vector<Contact> contResult;
    cp.push_back(firstBall->getPosition() + rhat*firstBall->getRadius());

    //sf::CircleShape circ1{2.0f};
    //circ1.setPosition(cp[0]);
    //sf::CircleShape circ2{2.0f};
    //circ2.setPosition(*cp.end());

    //debugWindow->draw(circ1);
    //debugWindow->draw(circ2);

    float separation = sfVectorMath::dot(penetVector, rhat);
    if(separation <= 0.0f)
    {
        Collisions::generateContacts(firstBall, secondBall, contResult, cp, rhat, separation);
        firstBall->incTimesCollided();
        secondBall->incTimesCollided();
    }
    return contResult;
}

std::vector<Contact> Collisions::collisionBallAABB(Ball* origBall, AABB* origAABB)
{
    //std::cout << "Collisions\n";
    sf::Vector2f rBall = origBall->getPosition();
    //sf::Vector2f v = origBall->getVelocity() - origAABB->getVelocity();

    //float coefRest = 0.7f;
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


    std::vector<sf::Vertex > rect1Vert = origAABB->constructVerts();
    ClippedPoints cp = Collisions::getContactPoints(rect1Vert, *origBall, contactNormal);


    //Collisions::applyImpulse(origBall, origAABB, contactNormal, penetVector, cp);

}

std::vector<Contact> Collisions::collisionAABBAABB(AABB* rect1, AABB* rect2)
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

std::vector<Contact> Collisions::collisionBallOBB(Ball* ball, OBB* rect)
{

        float rotAngle = rect->getRotAngle();
        sf::Rect<float > rectBounds = rect->getGlobalBounds();

        sf::Vector2f ballVelocity = sfVectorMath::rotate(ball->getVelocity(), -rotAngle);
        sf::Vector2f rectVelocity = sfVectorMath::rotate(rect->getVelocity(), -rotAngle);
        sf::Vector2f ballPosition = sfVectorMath::rotate(ball->getPosition(), -rotAngle);
        sf::Vector2f rectPosition = sfVectorMath::rotate(rect->getPosition(), -rotAngle)
                                  - sf::Vector2f{rectBounds.width/2.0f, rectBounds.height/2.0f};


        AABB obbInFrame{{rectPosition,
                        rectVelocity,
                        {rectBounds.width, rectBounds.height},
                        rect->getMass(), 0.0f, 0.0f, 0.0f, 0.0f
                        }};

        Ball ballInFrame{{ballPosition,
                        ballVelocity,
                        {ball->getRadius(), 0.0f},
                        rect->getMass(), 0.0f, 0.0f, 0.0f, 0.0f
                        }};

        ballInFrame.addRotRate(ball->getRotRate());
        obbInFrame.addRotRate(rect->getRotRate());

        std::pair<sf::Vector2f, sf::Vector2f> contact = Collisions::getContactNormal(&ballInFrame, &obbInFrame);

        sf::Vector2f contactNorm = sfVectorMath::rotate(contact.first, rotAngle);
        sf::Vector2f cornerPos = sfVectorMath::rotate(contact.second, rotAngle);

        //std::cout << contactNorm << " norm\n";
        //std::cout << penetVector << " pen\n";

        sf::Vector2f contactPoint = ball->getPosition() + contactNorm*ball->getRadius();
        ClippedPoints cp;
        cp.push_back(contactPoint);

        //std::vector<sf::Vertex > rectVerts = rect->constructVerts();
        //ClippedPoints cp = Collisions::getContactPoints(rectVerts, *ball, contactNorm);
        //Collisions::applyImpulse(ball, rect, contactNorm, penetVector, cp);
}

std::vector<Contact> Collisions::collisionOBBOBB(OBB* rect1, OBB* rect2)
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

    //penetVector += 0.1f*contactNorm;

    ClippedPoints cp = Collisions::getContactPoints(rect1Vert, rect2Vert, contactNorm);

    //sf::CircleShape circ1{2.0f};
    //circ1.setPosition(*cp.begin());
    //sf::CircleShape circ2{2.0f};
    //circ2.setPosition(*cp.end());

    //debugWindow->draw(circ1);
    //debugWindow->draw(circ2);
    //if(cp.size()>0)
        //Collisions::applyImpulse(rect1, rect2, -contactNorm, -penetVector, cp);

}

std::vector<Contact> Collisions::collisionOBBPoly(OBB *rect, Polygon *poly)
{
    std::vector<sf::Vertex > rectVert = rect->constructVerts();
    std::vector<sf::Vertex > polyVert = poly->constructVerts();

    /*sf::VertexArray quad1(sf::TriangleStrip, 4);
    for(int i=0; i<3; ++i)
        quad1[i] = rect1Vert[i];

    sf::VertexArray quad2(sf::LineStrip, 4);
    for(int i=0; i<3; ++i)
        quad2[i] = rect2Vert[i];

    debugWindow->draw(quad1);
    debugWindow->draw(quad2);*/

    sf::Vector2f penetVector = Collisions::sepAxisTest(rectVert, polyVert).second;
    sf::Vector2f contactNorm = sfVectorMath::norm(penetVector);

    penetVector += 0.1f*contactNorm;

    ClippedPoints cp = Collisions::getContactPoints(rectVert, polyVert, contactNorm);

    //sf::CircleShape circ1{2.0f};
    //circ1.setPosition(*cp.begin());
    //sf::CircleShape circ2{2.0f};
    //circ2.setPosition(*cp.end());

    //debugWindow->draw(circ1);
    //debugWindow->draw(circ2);
        //Collisions::applyImpulse(rect, poly, -contactNorm, -penetVector, cp);
}

std::vector<Contact> Collisions::collisionBallPoly(Ball *ball, Polygon *poly)
{
    std::pair<sf::Vector2f, sf::Vector2f> contact = Collisions::getContactNormal(ball, poly);

    sf::Vector2f contactNorm = -contact.first;
    sf::Vector2f cornerPos = contact.second;
    sf::Vector2f penetVector = Collisions::calcPenetVector(cornerPos, contactNorm, *ball);
    //std::cout << penetVector << " pen\n";

    sf::Vector2f contactPoint = ball->getPosition() + contactNorm*ball->getRadius();
    ClippedPoints cp;
    cp.push_back(contactPoint);

    //sf::CircleShape circ1{2.0f};
    //circ1.setPosition(contactPoint);
    //sf::CircleShape circ2{2.0f};
    //circ2.setPosition(*cp.end());

    //debugWindow->draw(circ1);
    //debugWindow->draw(circ2);

    std::vector<Contact> contResult;

    float separation = sfVectorMath::dot(penetVector, contactNorm);
    if(separation <= 0.0f)
        Collisions::generateContacts(ball, poly, contResult, cp, contactNorm, separation);
    return contResult;
}

std::vector<Contact> Collisions::collisionPolyPoly(Polygon* poly1, Polygon *poly2)
{
    std::vector<sf::Vertex > poly1Vert = poly1->constructVerts();
    std::vector<sf::Vertex > poly2Vert = poly2->constructVerts();

    /*sf::VertexArray quad1(sf::TriangleStrip, 4);
    for(int i=0; i<3; ++i)
        quad1[i] = rect1Vert[i];

    sf::VertexArray quad2(sf::LineStrip, 4);
    for(int i=0; i<3; ++i)
        quad2[i] = rect2Vert[i];

    debugWindow->draw(quad1);
    debugWindow->draw(quad2);*/
    std::vector<Contact> contResult;

    std::pair<bool, sf::Vector2f> sepAxis = Collisions::sepAxisTest(poly1Vert, poly2Vert);

    if(sepAxis.first)
    {
        sf::Vector2f penetVector = sepAxis.second;
        sf::Vector2f contactNorm = -sfVectorMath::norm(penetVector);

        //std::cout << contactNorm << " hello\n";
        //float redMass = 1.0f/(1.0f/poly1->getMass() + 1.0f/poly2->getMass());
        //penetVector += 0.1f*contactNorm;

        ClippedPoints cp = Collisions::getContactPoints(poly1Vert, poly2Vert, contactNorm);

        /*sf::CircleShape circ1{2.0f};
        circ1.setPosition(*cp.begin());
        sf::CircleShape circ2{2.0f};
        circ2.setPosition(*cp.end());*/
        sf::CircleShape circ3{5.0f};
        circ3.setPosition(poly1->getPosition());

        //debugWindow->draw(circ1);
        //debugWindow->draw(circ2);
        debugWindow->draw(circ3);

        float separation = sfVectorMath::dot(penetVector, contactNorm);

        for(int i=0; i<(int)cp.size(); ++i)
        {
            Contact tempContact;
            tempContact.normal = contactNorm;
            tempContact.position = cp[i];
            tempContact.rA = cp[i] - poly1->getPosition();
            tempContact.rB = cp[i] - poly2->getPosition();
            tempContact.separation = separation;

            contResult.push_back(tempContact);
        }
    }
    return contResult;
}
