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
    sf::Rect<float > rect1 = p1->getBoundingBox();
    sf::Rect<float > rect2 = p2->getBoundingBox();

    return rect1.intersects(rect2);
}

std::vector<Contact > Collisions::genericCollision(PhysicsObject* p1,
                                                   PhysicsObject* p2)
{
    std::vector<Contact > contResult;
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

            for(int i=0; i<(int)cp.size(); ++i)
            {
                Contact tempContact;
                tempContact.normal = contactNorm;
                tempContact.position = cp[i];
                tempContact.rA = cp[i] - p1->getPosition();
                tempContact.rB = cp[i] - p2->getPosition();
                tempContact.separation = separation;

                contResult.push_back(tempContact);
            }
        }
    }


    return contResult;
}
