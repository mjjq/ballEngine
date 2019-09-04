#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <cassert>

#include "collisionDetection.h"
#include "Math.h"
#include "stringConversion.h"
#include "constraintSolver.h"



bool Collisions::isAABBIntersecting(PhysicsObject* p1, PhysicsObject* p2)
{
    return (p1->getBoundingSphere()).intersects(p2->getBoundingSphere());
}

std::vector<Contact > Collisions::genericCollision(PhysicsObject* p1,
                                                   PhysicsObject* p2)
{
    std::vector<Contact > contResult;

    if(!isAABBIntersecting(p1, p2))
        return contResult;

    ClippedPoints cp;

    if(p1->getVertexCount() < 3 || p2->getVertexCount() < 3)
    {
        Edge closestLine = GJK::getClosestPoints(p1, p2);

        sf::Vector2f sepVector = p2->getPosition() - p1->getPosition();
        float p1Radius = p1->getRadius();
        float p2Radius = p2->getRadius();
        float totalRadius = p1Radius + p2Radius;
        float lineLengthSq = Math::square(closestLine.dir);

        if(lineLengthSq <= totalRadius*totalRadius)
        {
            sf::Vector2f contactNorm = Math::norm(closestLine.dir);

            float separation = sqrtf(lineLengthSq) - totalRadius;

            cp.push_back(closestLine.v1 + p1Radius * contactNorm);

            Collisions::generateContacts(p1, p2, contResult, cp, contactNorm, separation);
        }
    }
    else
    {
        std::vector<sf::Vertex > p1Verts = p1->constructVerts();
        std::vector<sf::Vertex > p2Verts = p2->constructVerts();

        std::pair<bool, sf::Vector2f > sepAxis = sepAxisTest(p1Verts, p2Verts);

        if(sepAxis.first)
        {
            sf::Vector2f penetVector = sepAxis.second;
            sf::Vector2f contactNorm = -Math::norm(penetVector);

            ClippedPoints cp = Collisions::getContactPoints(p1Verts,
                                                            p2Verts,
                                                            contactNorm);

            float separation = Math::dot(penetVector, contactNorm);

            Collisions::generateContacts(p1, p2, contResult, cp, contactNorm, separation);
        }
    }


    return contResult;
}

void Collisions::genericCollision(ConcavePolygonWrap & p1,
                                  ConcavePolygonWrap & p2,
                                  std::vector<Contact > & contResult)
{
    //if(!isAABBIntersecting(p1, p2))
    //    return contResult;

    ClippedPoints cp;

    if(p1.getPointCount() < 3 || p2.getPointCount() < 3)
    {
        Edge closestLine = GJK::getClosestPoints(p1, p2);

        sf::Vector2f sepVector = p2.getPosition() - p1.getPosition();
        float p1Radius = p1.getRadius();
        float p2Radius = p2.getRadius();
        float totalRadius = p1Radius + p2Radius;
        float lineLengthSq = Math::square(closestLine.dir);

        /*sf::CircleShape circ(3.0f);
        circ.setOrigin({3.0f, 3.0f});
        circ.setPosition(closestLine.v1);
        debugWindow->draw(circ);
        circ.setPosition(closestLine.v2);
        debugWindow->draw(circ);
        std::vector<sf::Vertex > verts(2);
        verts[0].position = closestLine.v1;
        verts[1].position = closestLine.v2;*/

        //debugWindow->draw(verts.data(), verts.size(), sf::Lines);

        if(lineLengthSq <= totalRadius*totalRadius)
        {
            sf::Vector2f contactNorm = Math::norm(closestLine.dir);

            float separation = sqrtf(lineLengthSq) - totalRadius;

            cp.push_back(closestLine.v1 + p1Radius * contactNorm);

            Collisions::generateContacts(p1, p2, contResult, cp, contactNorm, separation);
        }
    }
    else
    {
        std::vector<sf::Vertex > p1Verts = p1.getVertices(true);
        std::vector<sf::Vertex > p2Verts = p2.getVertices(true);

        std::pair<bool, sf::Vector2f > sepAxis = sepAxisTest(p1Verts, p2Verts);

        if(sepAxis.first)
        {
            sf::Vector2f penetVector = sepAxis.second;
            sf::Vector2f contactNorm = -Math::norm(penetVector);

            ClippedPoints cp = Collisions::getContactPoints(p1Verts,
                                                            p2Verts,
                                                            contactNorm);

            float separation = Math::dot(penetVector, contactNorm);

            Collisions::generateContacts(p1, p2, contResult, cp, contactNorm, separation);
        }
    }
}

std::vector<Contact > Collisions::findCollisions(PhysicsObject* p1,
                                                 PhysicsObject* p2)
{
    std::vector<Contact > contResult;

    if(p1 == nullptr || p2 == nullptr)
        return contResult;

    ConcavePolygonWrap & p1Geometry = p1->getGeometry();
    ConcavePolygonWrap & p2Geometry = p2->getGeometry();

    if(!p1Geometry.getBounds().intersects(p2Geometry.getBounds()))
        return contResult;


    for(int i=0; i<p1Geometry.getConvexPolyCount(); ++i)
        for(int j=0; j<p2Geometry.getConvexPolyCount(); ++j)
        {

//            if(p1Geometry.getConvexPoly(i).getBounds().
//               intersects(p2Geometry.getConvexPoly(j).getBounds()))
            {
                genericCollision(p1Geometry.getConvexPoly(i),
                                 p2Geometry.getConvexPoly(j),
                                 contResult);
            }
        }

    return contResult;
}
