/**
    @class classPolygon
    Purpose: A class which represents a physical simulation ball. Contains
    functions for forces, updating velocity and position as well as basic
    collisions.

    @author mjjq
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "classPolygon.h"
#include "sfVectorMath.h"
#include "integrators.h"
#include "stringConversion.h"

/**
    Construct the ball.

    @param radius The radius of the ball.
    @param mass The mass of the ball.
    @param initPos The initial position of the ball.
    @param initVel The initial velocity of the ball.
*/
Polygon::Polygon(ObjectProperties init) :
PhysicsObject(init),
         vertices{init._vertices}
{
    centrePosition = sf::Vector2f{0.0f, 0.0f};
    for(sf::Vertex &vert : vertices)
    {
        centrePosition += vert.position;
    }
    centrePosition = centrePosition/static_cast<float>(init._vertices.size());
    for(sf::Vertex &vert : vertices)
    {
        vert.position = vert.position - centrePosition;
    }

    centreOfMass = {0.0f, 0.0f}; //sf::Vector2f{size.x/2.0f, size.y/2.0f};

    for(sf::Vertex &vert : vertices)
    {
        momentInertia += sfVectorMath::square(vert.position - centreOfMass);
    }
    momentInertia = momentInertia*init._mass/init._vertices.size();

    //rotAngle = init._rotation;
    //rotRate = init._rotRate;

    for(unsigned int i=0; i<vertices.size(); ++i)
    {
        sf::Vector2f edge = vertices[(i+1)%vertices.size()].position - vertices[i].position;
        edgeNorms.push_back(sfVectorMath::norm(sfVectorMath::orthogonal(edge, 1.0f)));
    }

    sf::Vector2f norm1 = edgeNorms[0];
    sf::Vector2f norm2 = edgeNorms[edgeNorms.size()-1];
    float denom = sqrt(0.5f*(1+sfVectorMath::dot(norm1, norm2)));
    edgeTotals.push_back(sfVectorMath::norm(norm1+norm2)/denom);

    for(unsigned int i=1; i<edgeNorms.size(); ++i)
    {
        norm1 = edgeNorms[i-1];
        norm2 = edgeNorms[i];
        denom = sqrt(0.5f*(1+sfVectorMath::dot(norm1, norm2)));
        edgeTotals.push_back(sfVectorMath::norm(norm1+norm2)/denom);
    }

    genBoundingOBB();
}

Polygon::~Polygon() {}

ObjectType Polygon::type() const { return MY_TYPE; }

void Polygon::draw(sf::RenderWindow &_window)
{
    int numVerts = vertices.size();
    std::vector<sf::Vertex> temp;
    sf::ConvexShape polygon;
    polygon.setPointCount(numVerts);
    for(int i=0; i<numVerts; ++i)
    {
        sf::Vector2f vertPos = sfVectorMath::rotate(vertices.at(i).position, rotAngle) + position;
        polygon.setPoint(i, vertPos);
        //temp.push_back(sf::Vertex(vertPos));
    }
    polygon.setOutlineThickness(-2);
    polygon.setOutlineColor(sf::Color::Red);
    polygon.setFillColor({80,80,80,80});


    //_window.draw(temp.data(), numVerts, sf::Lines);
    _window.draw(polygon);
}

float Polygon::getMinSize()
{
    return ((size.x < size.y) ? size.x/2.0f : size.y/2.0f);
}


sf::Rect<float > Polygon::getGlobalBounds()
{
    return boundingOBB;
}

sf::Rect<float > Polygon::getBoundingBox()
{
    float maxX = farthestPointInDir({1.0f, 0.0f}).position.x;
    float maxY = farthestPointInDir({0.0f, 1.0f}).position.y;
    float minX = farthestPointInDir({-1.0f, 0.0f}).position.x;
    float minY = farthestPointInDir({0.0f, -1.0f}).position.y;

    sf::Rect<float > rect{minX, minY, maxX-minX, maxY-minY};
    return rect;
}

std::vector<sf::Vertex > Polygon::constructVerts()
{
    std::vector<sf::Vertex > tFormedVerts;
    for(sf::Vertex &vert : vertices)
    {
        sf::Vector2f newPos = sfVectorMath::rotate(vert.position, rotAngle);
        newPos += position;
        tFormedVerts.push_back(sf::Vertex(newPos));
    }
    return tFormedVerts;
}

std::vector<sf::Vertex > Polygon::constructLocalVerts()
{
    return vertices;
}

float Polygon::getRotAngle()
{
    return rotAngle;
}

void Polygon::genBoundingOBB()
{
    float minX = 1e+20;
    float maxX = -minX;
    float minY = minX;
    float maxY = -minX;
    for(sf::Vertex &vert : vertices)
    {
        if(vert.position.x < minX)
            minX = vert.position.x;
        if(vert.position.x > maxX)
            maxX = vert.position.x;
        if(vert.position.y < minY)
            minY = vert.position.y;
        if(vert.position.y > maxY)
            maxY = vert.position.y;
    }

    size = {maxX-minX, maxY-minY};
    //boundingOBB.clear();
    boundingOBB = sf::Rect<float>(minX, minY, maxX-minX, maxY-minY);

}

std::vector<sf::Vector2f> Polygon::getLocalEdgeNorms()
{
    return edgeNorms;
}

std::vector<sf::Vector2f> Polygon::getLocalEdgeTotals()
{
    return edgeTotals;
}

sf::Vertex Polygon::farthestPointInDir(sf::Vector2f direction)
{
    std::vector<sf::Vertex > obbVerts = constructVerts();
    sf::Vertex result;

    float maxProj = -1e15;
    for(unsigned int i=0; i<obbVerts.size(); ++i)
    {
        float currProj = sfVectorMath::dot(obbVerts[i].position, direction);
        if(currProj > maxProj)
        {
            maxProj = currProj;
            result = obbVerts[i];
        }
    }

    return result;
}
