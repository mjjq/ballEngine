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
REGISTER_RESOLVE_FUNCTION(OBB, Polygon,  &Collisions::collisionOBBPoly)
REGISTER_RESOLVE_FUNCTION(Ball, Polygon,   &Collisions::collisionBallPoly)
REGISTER_RESOLVE_FUNCTION(Polygon, Polygon,   &Collisions::collisionPolyPoly)


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

    //float coefRest = 0.9f;
    //sf::Vector2f relPos = firstBall->getPosition() - secondBall->getPosition();
    sf::Vector2f rhat = norm(firstBall->getPosition() - secondBall->getPosition());

    //sf::Vector2f v1 = firstBall->getVelocity();
    //sf::Vector2f v2 = secondBall->getVelocity();
    float m1 = firstBall->getMass();
    float m2 = secondBall->getMass();

    sf::Vector2f penetVector = Collisions::calcPenetVector(firstBall, secondBall);
    sf::Vector2f penetVector1 = penetVector*m2/(m1+m2);
    sf::Vector2f penetVector2 = penetVector*m1/(m1+m2);

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

    firstBall->setPosition(firstBall->getPosition() - penetVector1);
    secondBall->setPosition(secondBall->getPosition() + penetVector2);
}

void Collisions::collisionBallAABB(Ball* origBall, AABB* origAABB)
{
    //std::cout << "Collisions\n";
    sf::Vector2f rBall = origBall->getPosition();
    sf::Vector2f rAABB = origAABB->getPosition();
    //sf::Vector2f v = origBall->getVelocity() - origAABB->getVelocity();

    float redMassBall = origAABB->getMass()/(origAABB->getMass() + origBall->getMass());
    float redMassAABB = origBall->getMass()/(origAABB->getMass() + origBall->getMass());

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


    if(cp.size() > 0)
        Collisions::applyImpulse(origBall, origAABB, contactNormal, cp);

    //std::cout << cp.size() << "\n";
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

        Ball ballInFrame{ball->getRadius(), ball->getMass(), ballPosition, ballVelocity};
        ballInFrame.addRotRate(ball->getRotRate());
        obbInFrame.addRotRate(rect->getRotRate());

        std::pair<sf::Vector2f, sf::Vector2f> contact = Collisions::getContactNormal(&ballInFrame, &obbInFrame);

        sf::Vector2f contactNorm = sfVectorMath::rotate(contact.first, rotAngle);
        sf::Vector2f cornerPos = sfVectorMath::rotate(contact.second, rotAngle);
        sf::Vector2f penetVector = Collisions::calcPenetVector(cornerPos, contactNorm, *ball);

        //std::cout << contactNorm << " norm\n";
        //std::cout << penetVector << " pen\n";

        sf::Vector2f contactPoint = ball->getPosition() - contactNorm*ball->getRadius();
        ClippedPoints cp;
        cp.push_back(contactPoint);

        //std::vector<sf::Vertex > rectVerts = rect->constructVerts();
        //ClippedPoints cp = Collisions::getContactPoints(rectVerts, *ball, contactNorm);
        Collisions::applyImpulse(ball, rect, contactNorm, cp);

        float redMassRect = ball->getMass()/(ball->getMass() + rect->getMass());
        float redMassBall = rect->getMass()/(ball->getMass() + rect->getMass());

        assert(!std::isnan(sfVectorMath::square(penetVector)));
        rect->setPosition(rect->getPosition() + redMassRect*penetVector);
        ball->setPosition(ball->getPosition() - redMassBall*penetVector);
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

    float redMass = 1.0f/(1.0f/rect1->getMass() + 1.0f/rect2->getMass());
    penetVector += 0.1f*contactNorm;

    ClippedPoints cp = Collisions::getContactPoints(rect1Vert, rect2Vert, contactNorm);

    //sf::CircleShape circ1{2.0f};
    //circ1.setPosition(*cp.begin());
    //sf::CircleShape circ2{2.0f};
    //circ2.setPosition(*cp.end());

    //debugWindow->draw(circ1);
    //debugWindow->draw(circ2);
    if(cp.size()>0)
        Collisions::applyImpulse(rect1, rect2, contactNorm, cp);

    rect1->setPosition(rect1->getPosition() + redMass*penetVector/rect1->getMass());
    rect2->setPosition(rect2->getPosition() - redMass*penetVector/rect2->getMass());

}

void Collisions::collisionOBBPoly(OBB *rect, Polygon *poly)
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

    float redMass = 1.0f/(1.0f/rect->getMass() + 1.0f/poly->getMass());
    penetVector += 0.1f*contactNorm;

    ClippedPoints cp = Collisions::getContactPoints(rectVert, polyVert, contactNorm);

    //sf::CircleShape circ1{2.0f};
    //circ1.setPosition(*cp.begin());
    //sf::CircleShape circ2{2.0f};
    //circ2.setPosition(*cp.end());

    //debugWindow->draw(circ1);
    //debugWindow->draw(circ2);
    if(cp.size()>0)
        Collisions::applyImpulse(rect, poly, contactNorm, cp);

    //std::cout << contactNorm << "norm\n";
    rect->setPosition(rect->getPosition() + redMass*penetVector/rect->getMass());
    poly->setPosition(poly->getPosition() - redMass*penetVector/poly->getMass());
}

void Collisions::collisionBallPoly(Ball *ball, Polygon *poly)
{
    std::pair<sf::Vector2f, sf::Vector2f> contact = Collisions::getContactNormal(ball, poly);

    sf::Vector2f contactNorm = contact.first;
    sf::Vector2f cornerPos = contact.second;
    sf::Vector2f penetVector = Collisions::calcPenetVector(cornerPos, contactNorm, *ball);

    std::cout << sqrt(sfVectorMath::square(cornerPos - ball->getPosition())) << " cDist\n";
    std::cout << ball->getPosition() << " ballPos\n";
    std::cout << penetVector << " pen\n";

    //std::cout << contactNorm << " norm\n";
    //std::cout << penetVector << " pen\n";

    sf::Vector2f contactPoint = ball->getPosition() - contactNorm*ball->getRadius();
    ClippedPoints cp;
    cp.push_back(contactPoint);


    sf::CircleShape circ1{2.5f};
    circ1.setPosition(contactPoint);
    circ1.setOrigin({2.5f, 2.5f});
    //std::cout << origPoly[vertexColl].position << "\n";
    debugWindow->draw(circ1);

    if(cp.size() > 0)
        Collisions::applyImpulse(ball, poly, contactNorm, cp);

    float redMassPoly = ball->getMass()/(ball->getMass() + poly->getMass());
    float redMassBall = poly->getMass()/(ball->getMass() + poly->getMass());

    assert(!std::isnan(sfVectorMath::square(penetVector)));
    ball->setPosition(ball->getPosition() - redMassBall*penetVector);
    poly->setPosition(poly->getPosition() + redMassPoly*penetVector);

}

void Collisions::collisionPolyPoly(Polygon* poly1, Polygon *poly2)
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

    sf::Vector2f penetVector = Collisions::sepAxisTest(poly1Vert, poly2Vert).second;
    sf::Vector2f contactNorm = sfVectorMath::norm(penetVector);

    float redMass = 1.0f/(1.0f/poly1->getMass() + 1.0f/poly2->getMass());
    penetVector += 0.1f*contactNorm;

    ClippedPoints cp = Collisions::getContactPoints(poly1Vert, poly2Vert, contactNorm);

    //sf::CircleShape circ1{2.0f};
    //circ1.setPosition(*cp.begin());
    //sf::CircleShape circ2{2.0f};
    //circ2.setPosition(*cp.end());

    //debugWindow->draw(circ1);
    //debugWindow->draw(circ2);
    if(cp.size()>0)
        Collisions::applyImpulse(poly1, poly2, contactNorm, cp);

    //std::cout << contactNorm << "norm\n";
    poly1->setPosition(poly1->getPosition() + redMass*penetVector/poly1->getMass());
    poly2->setPosition(poly2->getPosition() - redMass*penetVector/poly2->getMass());
}



std::pair<sf::Vector2f, sf::Vector2f> Collisions::getContactNormal(Ball *origBall, AABB *origAABB)
{
    //std::cout << "Collisions\n";
    sf::Vector2f rBall = origBall->getPosition();
    //sf::Vector2f rAABB = origAABB->getPosition();
    //sf::Vector2f v = origBall->getVelocity() - origAABB->getVelocity();

    //float redMassBall = origAABB->getMass()/(origAABB->getMass() + origBall->getMass());
    //float redMassAABB = origBall->getMass()/(origAABB->getMass() + origBall->getMass());

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

    //sf::Vector2f penetVector = {0.0f, 0.0f};
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

    return std::make_pair(contactNormal, cornerPos);
}

std::pair<sf::Vector2f, sf::Vector2f> Collisions::getContactNormal(Ball* ball, Polygon* poly)
{
    std::vector<sf::Vertex > verts = poly->constructVerts();
    sf::Vector2f relPos     = {0.0f, 0.0f};
    sf::Vector2f relPos2    = {0.0f, 0.0f};
    sf::Vector2f edgeNorm   = {0.0f, 0.0f};
    sf::Vector2f cornerPos  = {0.0f, 0.0f};

    Edge bestEdge;
    float ballRad = ball->getRadius();

    float distance = 1e+15;
    float minVertDistSq = 1e+15;

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

        if(normDist <= distance && normDist >= ball->getRadius())
        {
            if(tanDist >= 0.0f && tanDist*tanDist <= segDistSq)
            {
                distance = normDist;
                edgeNorm = norm;
                cornerPos = verts[i].position;
                i+=1;

            }
        }
        else if(normDist <= distance && normDist >= 0.0f &&
                vertDistSq <= minVertDistSq)
        {
            minVertDistSq = vertDistSq;
            cornerPos = verts[i].position;
            edgeNorm = sfVectorMath::norm(ball->getPosition() -
                                          cornerPos);
        }

    }

    return std::make_pair(sfVectorMath::norm(edgeNorm), cornerPos);
}


void Collisions::applyImpulse(PhysicsObject *obj1,
                              PhysicsObject *obj2,
                              sf::Vector2f contactNorm,
                              ClippedPoints &collisionPoints)
{
    sf::Vector2f relVel = obj1->getVelocity() - obj2->getVelocity();
    for(sf::Vector2f &collPoint : collisionPoints)
    {
        sf::Vector2f rA = collPoint - obj1->getCoM();
        sf::Vector2f rB = collPoint - obj2->getCoM();
        relVel -= Collisions::orthogonal(rA, obj1->getRotRate()) -
                    Collisions::orthogonal(rB, obj2->getRotRate());
    }
    //relVel = relVel/static_cast<float>(collisionPoints.size());

    //std::cout << relVel << "rel\n";
    //std::cout << collisionPoints.size() << " cp\n";



    float coefRest = 0.5f;
    float mu = 0.3f;
    //if(std::abs(sfVectorMath::dot(relVel, contactNorm)) < 5.0f)
    //{
    //    coefRest = coefRest * pow(std::abs(sfVectorMath::dot(relVel, contactNorm)) / 5.0f, 1);
        //std::cout << coefRest << " coef\n";
    //}
    //std::cout << contactNorm << "\n";
    sf::Vector2f contactTangent = relVel - sfVectorMath::dot(relVel, contactNorm)*contactNorm;
    if(sfVectorMath::square(contactTangent) > 0.0f)
        contactTangent = sfVectorMath::norm(contactTangent);



    //std::cout << contactNorm << " n\n";
    //std::cout << contactTangent << " t\n";

    float j = 0.0f;
    float jt = 0.0f;

    sf::Vector2f resVectorA = sf::Vector2f{0.0f, 0.0f};
    sf::Vector2f resVectorB = sf::Vector2f{0.0f, 0.0f};

    float IA = obj1->getMomentInertia();
    float IB = obj2->getMomentInertia();

    for(unsigned int i=0; i<collisionPoints.size(); ++i)
    {
        sf::Vector2f rA = collisionPoints[i] - obj1->getCoM();

        sf::Vector2f rB = collisionPoints[i] - obj2->getCoM();

        resVectorA += rA;
        resVectorB += rB;
        /*float cTermA = pow( sfVectorMath::cross(rA, contactNorm), 2 )/IA;
        float cTermB = pow( sfVectorMath::cross(rB, contactNorm), 2 )/IB;

        float denom = (1.0f/obj1->getMass() + 1.0f/obj2->getMass() + cTermA + cTermB);

        j -= (1.0f+coefRest)*sfVectorMath::dot(relVel, contactNorm)/denom;

        cTermA = pow( sfVectorMath::cross(rA, contactTangent), 2 )/IA;
        cTermB = pow( sfVectorMath::cross(rB, contactTangent), 2 )/IB;

        denom = (1.0f/obj1->getMass() + 1.0f/obj2->getMass() + cTermA + cTermB);

        jt -= sfVectorMath::dot(relVel, contactTangent)/denom;*/

    }
    //resVectorA = resVectorA/static_cast<float>(collisionPoints.size());
    //resVectorB = resVectorB/static_cast<float>(collisionPoints.size());

    float cTermA = pow( sfVectorMath::cross(resVectorA, contactNorm), 2 )/IA;
    float cTermB = pow( sfVectorMath::cross(resVectorB, contactNorm), 2 )/IB;

    float denom = (1.0f/obj1->getMass() + 1.0f/obj2->getMass() + cTermA + cTermB);

    j -= (1.0f+coefRest)*sfVectorMath::dot(relVel, contactNorm)/denom;

    cTermA = pow( sfVectorMath::cross(resVectorA, contactTangent), 2 )/IA;
    cTermB = pow( sfVectorMath::cross(resVectorB, contactTangent), 2 )/IB;

    denom = (1.0f/obj1->getMass() + 1.0f/obj2->getMass() + cTermA + cTermB);

    jt -= sfVectorMath::dot(relVel, contactTangent)/denom;

    //j = j/static_cast<float>(collisionPoints.size());
    //jt = jt/static_cast<float>(collisionPoints.size());

    //std::cout << obj1->getCoM() << "CoM\n";
    //std::cout << collisionPoints[0] << "cp\n";
    //std::cout << resVectorA << "resA\n";

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
        frictionImpulse = -j * contactTangent * mu;

        //std::cout << sfVectorMath::cross(resVectorA, contactTangent) << "rescrosstan\n";
        dwA = -sfVectorMath::cross(resVectorA, contactTangent) * j * mu/ IA;
        dwB = -sfVectorMath::cross(resVectorB, contactTangent) * j * mu/ IB;

    }


    //std::cout << frictionImpulse << "fric impulse\n";
    //    std::cout << impulse << "impulse\n";
    //std::cout << collisionPoints.size() << " cp\n";
    /*if(std::isnan(impulse.x))
    {
        std::cout << frictionImpulse << "fric impulse\n";
        std::cout << impulse << "impulse\n";
        std::cout << relVel << "rel\n";
        std::cout << collisionPoints.size() << " cp\n";
        std::cout << contactNorm << " n\n";
        std::cout << contactTangent << " t\n\n";
        std::cout << denom << "denom\n";
std::cout << IA << " " << IB << "inertia\n\n";


    }*/

    dwA += sfVectorMath::cross(resVectorA, contactNorm) * j / IA;
    dwB += sfVectorMath::cross(resVectorB, contactNorm) * j / IB;

    /*if(std::abs(sfVectorMath::dot(relVel, contactNorm)) < 1.0f)
    {
        std::cout << "slow\n";
        obj1->setRotRate(0.0f);
        obj2->setRotRate(0.0f);
    }*/
    //std::cout << impulse << " impulse\n";
    //std::cout << obj1->getVelocity() << " vel\n";
    //std::cout << collisionPoints.size() << " cp\n\n";

sf::CircleShape circ1{2.5f};
    circ1.setPosition(obj1->getCoM());
    circ1.setOrigin({1.25f, 1.25f});
    circ1.setFillColor(sf::Color::Red);
    debugWindow->draw(circ1);

    obj1->addSolvedVelocity((impulse + frictionImpulse)/obj1->getMass(),
                             (impulse + frictionImpulse)/obj1->getMass());
    obj2->addSolvedVelocity(-(impulse + frictionImpulse)/obj2->getMass(),
                             -(impulse + frictionImpulse)/obj2->getMass());


    //std::cout << obj1->getMomentInertia() << "\n";
    //if(std::abs(dwA) > 1e-4)
        obj1->addRotRate(dwA);
    //if(std::abs(dwB) > 1e-4)
        obj2->addRotRate(-dwB);
}
