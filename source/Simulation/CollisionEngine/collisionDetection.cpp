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



std::function<float(PhysicsObject*,PhysicsObject*)>
    TCollFunctionTable[(int)(ObjectType::_Count)][(int)(ObjectType::_Count)];
REGISTER_TCOLL_FUNCTION(Ball, Ball, &Collisions::timeToCollBallBall)
REGISTER_TCOLL_FUNCTION(Ball, AABB, &Collisions::timeToCollBallAABB)
REGISTER_TCOLL_FUNCTION(AABB, AABB, &Collisions::timeToCollAABBAABB)
REGISTER_TCOLL_FUNCTION(Ball, OBB,  &Collisions::timeToCollBallOBB)
REGISTER_TCOLL_FUNCTION(OBB, OBB,   &Collisions::timeToCollOBBOBB)
REGISTER_TCOLL_FUNCTION(OBB, Polygon, &Collisions::timeToCollOBBPoly)
REGISTER_TCOLL_FUNCTION(Ball, Polygon, &Collisions::timeToCollBallPoly)
REGISTER_TCOLL_FUNCTION(Polygon, Polygon, &Collisions::timeToCollPolyPoly)



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

sf::RenderWindow* Collisions::debugWindow = nullptr;
void Collisions::setDebugWindow(sf::RenderWindow &window)
{
    debugWindow = &window;
    std::cout << debugWindow << "\n";
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

    float epsilon = 1e-5;
    float tmin = -1.0f;
    float tmax = 10000.0f;

    tmin = Collisions::rayAABBIntersect(r, v, minkSumAABB, tmin, tmax, epsilon);
    if(std::isnan(tmin))
        return tmin;

    sf::Vector2f intPoint = r+v*tmin;
    sf::Rect<float> origAABBRect = origAABB->getGlobalBounds();


    if(origAABBRect.contains(r.x,r.y))
    {
        //return std::numeric_limits<float>::quiet_NaN();
    }

    bool intersectCorner = false;

    if(intPoint.x <= origAABBRect.left && intPoint.y <= origAABBRect.top)
    {
        tmin = raySphereIntersect(r,v, {origAABBRect.left, origAABBRect.top}, origBall->getRadius());
        intersectCorner = true;
    }

    else if(intPoint.x >= origAABBRect.left + origAABBRect.width && intPoint.y <= origAABBRect.top)
    {
        tmin = raySphereIntersect(r,v,
                                  {origAABBRect.left+origAABBRect.width,
                                   origAABBRect.top},
                                  origBall->getRadius());
        intersectCorner = true;
    }

    else if(intPoint.x <= origAABBRect.left && intPoint.y >= origAABBRect.top + origAABBRect.height)
    {
        tmin = raySphereIntersect(r,v,
                                  {origAABBRect.left,
                                   origAABBRect.top+origAABBRect.height},
                                  origBall->getRadius());
        intersectCorner = true;
    }

    else if(intPoint.x >= origAABBRect.left + origAABBRect.width &&
            intPoint.y >= origAABBRect.top + origAABBRect.height)
    {
        tmin = raySphereIntersect(r,v,
                                  {origAABBRect.left+origAABBRect.width,
                                   origAABBRect.top+origAABBRect.height},
                                  origBall->getRadius());
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


    //std::cout << "MinkAABB: " << AABB << "\n";

    float epsilon = 1e-5;
    float tmin = -1.0f;
    float tmax = 10000.0f;

    tmin = Collisions::rayAABBIntersect(r, v, minkSumAABB, tmin, tmax, epsilon);
    if(std::isnan(tmin))
        return tmin;

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

float Collisions::timeToCollBallOBB(Ball *ball, OBB* rect)
{
    //AABB boundingBox = rect->getBoundingBox();
    sf::Rect<float > boundingBox = rect->getBoundingBox();
    AABB boundingAABB{{{boundingBox.left, boundingBox.top},
                        rect->getVelocity(),
                        {boundingBox.width, boundingBox.height},
                        1.0f, 0.0f, 0.0f, 0.0f, 0.0f
                       }};

    //sf::RectangleShape drawable{{boundingBox.width, boundingBox.height}};
    //drawable.setPosition(boundingBox.left, boundingBox.top);
    //debugWindow->draw(drawable);

    float tmin = Collisions::timeToCollBallAABB(ball, &boundingAABB);

    if(tmin < 10.0f || boundingBox.contains(ball->getPosition()))
    {
        sf::Rect<float > rectBounds = rect->getGlobalBounds();
        float rotAngle = rect->getRotAngle();

        sf::Vector2f ballPos = sfVectorMath::rotate(ball->getPosition(), -rotAngle);
        sf::Vector2f ballVel = sfVectorMath::rotate(ball->getVelocity(), -rotAngle);
        sf::Vector2f rectPos = sfVectorMath::rotate(rect->getPosition(), -rotAngle) -
                               sf::Vector2f{rectBounds.width/2.0f, rectBounds.height/2.0f};
        sf::Vector2f rectVel = sfVectorMath::rotate(rect->getVelocity(), -rotAngle);
        //- sf::Vector2f{rectBounds.width/2.0f, rectBounds.height/2.0f}
        AABB obbInFrame{{rectPos,
                        rectVel,
                        {rectBounds.width, rectBounds.height},
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f
                        }};
        sf::Vector2f relVelocity = ball->getVelocity() - rect->getVelocity();
        sf::Vector2f sepVector = ball->getPosition() - rect->getPosition();
        relVelocity = sfVectorMath::rotate(relVelocity, -rect->getRotAngle());
        sepVector = sfVectorMath::rotate(sepVector, -rect->getRotAngle());

        Ball ballInFrame{{ballPos,
                        ballVel,
                        {ball->getRadius(), 0.0f},
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};

        //ballInFrame.draw(*debugWindow);
        //obbInFrame.draw(*debugWindow);

        tmin = Collisions::timeToCollBallAABB(&ballInFrame, &obbInFrame);
    }

    return tmin;
}


float Collisions::timeToCollOBBOBB(OBB* rect1, OBB* rect2)
{
    sf::Rect<float > boundingBox = rect1->getBoundingBox();
    AABB boundingAABB1{{{boundingBox.left, boundingBox.top},
                       rect1->getVelocity(),
                        {boundingBox.width, boundingBox.height},
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f
                      }};

    //sf::RectangleShape drawable1{{boundingBox.width, boundingBox.height}};
    //drawable1.setPosition(boundingBox.left, boundingBox.top);
    //debugWindow->draw(drawable1);

    boundingBox = rect2->getBoundingBox();
    AABB boundingAABB2{{{boundingBox.left, boundingBox.top},
                       rect2->getVelocity(),
                        {boundingBox.width, boundingBox.height},
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f
                      }};

    //sf::RectangleShape drawable2{{boundingBox.width, boundingBox.height}};
    //drawable2.setPosition(boundingBox.left, boundingBox.top);
    //debugWindow->draw(drawable2);

    float tmin = Collisions::timeToCollAABBAABB(&boundingAABB1, &boundingAABB2);
    if(tmin < 10.0f)
    {
        std::vector<sf::Vertex > rect1Vert = rect1->constructVerts();
        std::vector<sf::Vertex > rect2Vert = rect2->constructVerts();

        /*sf::VertexArray quad1(sf::LineStrip, 4);
        for(int i=0; i<3; ++i)
            quad1[i] = rect1Vert[i];

        sf::VertexArray quad2(sf::LineStrip, 4);
        for(int i=0; i<3; ++i)
            quad2[i] = rect2Vert[i];

        debugWindow->draw(quad1);
        debugWindow->draw(quad2);*/

        std::pair<bool, sf::Vector2f> result = Collisions::sepAxisTest(rect1Vert, rect2Vert);
        if(!result.first)
            return std::numeric_limits<float>::quiet_NaN();
        else
            return 0.0f;
    }

    return tmin;
}

float Collisions::timeToCollOBBPoly(OBB* rect, Polygon* poly)
{
    sf::Rect<float > boundingBox = rect->getBoundingBox();
    AABB boundingAABB1{{{boundingBox.left, boundingBox.top},
                       rect->getVelocity(),
                        {boundingBox.width, boundingBox.height},
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f
                      }};

    //sf::RectangleShape drawable1{{boundingBox.width, boundingBox.height}};
    //drawable1.setPosition(boundingBox.left, boundingBox.top);
    //debugWindow->draw(drawable1);
    /*sf::Vertex verts[] = {
        sf::Vertex{{boundingBox.left, boundingBox.top}},
        sf::Vertex{{boundingBox.left, boundingBox.top + boundingBox.height}},
        sf::Vertex{{boundingBox.left + boundingBox.width, boundingBox.top + boundingBox.height}},
        sf::Vertex{{boundingBox.left + boundingBox.width, boundingBox.top}}
    };
    debugWindow->draw(verts, 4, sf::Lines);*/

    boundingBox = poly->getBoundingBox();
    AABB boundingAABB2{{{boundingBox.left, boundingBox.top},
                       poly->getVelocity(),
                        {boundingBox.width, boundingBox.height},
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f
                      }};

    //sf::RectangleShape drawable2{{boundingBox.width, boundingBox.height}};
    //drawable2.setPosition(boundingBox.left, boundingBox.top);
    //debugWindow->draw(drawable2);

    float tmin = Collisions::timeToCollAABBAABB(&boundingAABB1, &boundingAABB2);
    if(tmin < 10.0f)
    {
        std::vector<sf::Vertex > rectVert = rect->constructVerts();
        std::vector<sf::Vertex > polyVert = poly->constructVerts();

        /*sf::VertexArray quad1(sf::LineStrip, 4);
        for(int i=0; i<3; ++i)
            quad1[i] = rect1Vert[i];

        sf::VertexArray quad2(sf::LineStrip, 4);
        for(int i=0; i<3; ++i)
            quad2[i] = rect2Vert[i];

        debugWindow->draw(quad1);
        debugWindow->draw(quad2);*/

        std::pair<bool, sf::Vector2f> result = Collisions::sepAxisTest(rectVert, polyVert);

        if(!result.first)
            return std::numeric_limits<float>::quiet_NaN();
        else
            return 0.0f;
    }

    return tmin;
}

float Collisions::timeToCollBallPoly(Ball* ball, Polygon* poly)
{
    sf::Rect<float > boundingBox = poly->getBoundingBox();
    AABB boundingAABB1{
        {{boundingBox.left, boundingBox.top},
            poly->getVelocity(),
            {boundingBox.width, boundingBox.height},
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f
          }
          };

    sf::Vertex verts[] = {
        sf::Vertex{{boundingBox.left, boundingBox.top}},
        sf::Vertex{{boundingBox.left, boundingBox.top + boundingBox.height}},
        sf::Vertex{{boundingBox.left + boundingBox.width, boundingBox.top + boundingBox.height}},
        sf::Vertex{{boundingBox.left + boundingBox.width, boundingBox.top}}
    };
    debugWindow->draw(verts, 4, sf::Lines);

    float t = Collisions::timeToCollBallAABB(ball, &boundingAABB1);
    if(t < 10.0f || boundingBox.contains(ball->getPosition()))
    {
        std::vector<sf::Vertex > origPoly = poly->constructVerts();
        std::vector<sf::Vertex > minkSum = poly->constructLocalVerts();
        std::vector<sf::Vector2f > edgeTotals = poly->getLocalEdgeTotals();
        float ballRad = ball->getRadius();

        for(unsigned int i=0; i<minkSum.size(); ++i)
        {
            sf::Vector2f delta = ball->getRadius() * edgeTotals.at(i);
            minkSum[i].position += delta;
            minkSum[i].position = sfVectorMath::rotate(minkSum[i].position, poly->getRotAngle());
            minkSum[i].position += poly->getPosition();

            sf::Vector2f relPos = ball->getPosition() - origPoly[i].position;

            if(sfVectorMath::square(relPos) < ballRad*ballRad)
                return 0.0f;
        }

        sf::Vector2f centreRelPos = ball->getPosition() - poly->getPosition();
        sf::Vector2f ballVel = ball->getVelocity() + sfVectorMath::orthogonal(centreRelPos, poly->getRotRate());
        t = Collisions::rayPolyIntersect(ball->getPosition(), ballVel-poly->getVelocity(),
                                               minkSum, -1e+15f, 1e+15f, 1e-15f);


        sf::Vertex intPoint{{ball->getPosition() + t*(ballVel-poly->getVelocity())}};

        std::vector<sf::Vertex > showIntVector;
        showIntVector.push_back({ball->getPosition()});
        showIntVector.push_back(intPoint);
        //debugWindow->draw(showIntVector.data(), showIntVector.size(), sf::LineStrip);

        sf::Vertex ballPos = {ball->getPosition()};

        int vertexColl = 0;
        if(!std::isnan(t) && t>=0.0f)
            vertexColl = Collisions::getClosestVertex(origPoly, intPoint);
        else
            vertexColl = Collisions::getClosestVertex(origPoly, ballPos);

        /*sf::CircleShape circ1{ballRad};
        circ1.setOrigin({ballRad, ballRad});
        circ1.setPosition(origPoly[vertexColl].position);
        debugWindow->draw(circ1);*/


        float minkSegLength = pow(ball->getRadius(), 2) *
                (sfVectorMath::square(edgeTotals[vertexColl]) - 1.0f );

        if(sfVectorMath::square(intPoint.position - minkSum[vertexColl].position) < minkSegLength ||
           sfVectorMath::square(ball->getPosition() - minkSum[vertexColl].position) < minkSegLength)
        {
            t = Collisions::raySphereIntersect(ball->getPosition(),
                                               ballVel-poly->getVelocity(),
                                               origPoly[vertexColl].position,
                                               ball->getRadius());
            if(t<0.0f)
                return std::numeric_limits<float>::quiet_NaN();
        }


        if(t<0.0f)
            return 0.0f;
    }
    return t;
}

float Collisions::timeToCollPolyPoly(Polygon* poly1, Polygon *poly2)
{
    sf::Rect<float > boundingBox = poly1->getBoundingBox();
    AABB boundingAABB1{{{boundingBox.left, boundingBox.top},
                       poly1->getVelocity(),
                        {boundingBox.width, boundingBox.height},
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f
                      }};

    //sf::RectangleShape drawable1{{boundingBox.width, boundingBox.height}};
    //drawable1.setPosition(boundingBox.left, boundingBox.top);
    //debugWindow->draw(drawable1);
    /*sf::Vertex verts[] = {
        sf::Vertex{{boundingBox.left, boundingBox.top}},
        sf::Vertex{{boundingBox.left, boundingBox.top + boundingBox.height}},
        sf::Vertex{{boundingBox.left + boundingBox.width, boundingBox.top + boundingBox.height}},
        sf::Vertex{{boundingBox.left + boundingBox.width, boundingBox.top}}
    };
    debugWindow->draw(verts, 4, sf::Lines);*/

    boundingBox = poly2->getBoundingBox();
    AABB boundingAABB2{{{boundingBox.left, boundingBox.top},
                       poly2->getVelocity(),
                        {boundingBox.width, boundingBox.height},
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f
                      }};

    //sf::RectangleShape drawable2{{boundingBox.width, boundingBox.height}};
    //drawable2.setPosition(boundingBox.left, boundingBox.top);
    //debugWindow->draw(drawable2);

    float tmin = Collisions::timeToCollAABBAABB(&boundingAABB1, &boundingAABB2);
    if(tmin < 10.0f)
    {
        std::vector<sf::Vertex > poly1Vert = poly1->constructVerts();
        std::vector<sf::Vertex > poly2Vert = poly2->constructVerts();

        /*sf::VertexArray quad1(sf::LineStrip, 4);
        for(int i=0; i<3; ++i)
            quad1[i] = rect1Vert[i];

        sf::VertexArray quad2(sf::LineStrip, 4);
        for(int i=0; i<3; ++i)
            quad2[i] = rect2Vert[i];

        debugWindow->draw(quad1);
        debugWindow->draw(quad2);*/

        std::pair<bool, sf::Vector2f> result = Collisions::sepAxisTest(poly1Vert, poly2Vert);

        if(!result.first)
            return std::numeric_limits<float>::quiet_NaN();
        else
            return 0.0f;
    }

    return tmin;
}
