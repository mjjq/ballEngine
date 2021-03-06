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
#include "Math.h"
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
PhysicsObject(init)
{

    momentInertia = 0.0f;

    if(init.type == ObjectType::Polygon ||
       init.type == ObjectType::ConcavePoly)
    {
        for(sf::Vertex &vert : init._vertices)
        {
            momentInertia += Math::square(vert.position);
        }
        momentInertia = momentInertia*init._mass/init._vertices.size();
        poly = ConcavePolygonWrap(Math::averageVertices(init._vertices));
    }
    else if(init.type == ObjectType::Ball)
    {
        momentInertia = 0.5f*init._mass*Math::square(init._size);
        poly = ConcavePolygonWrap({sf::Vertex{{0.0f, 0.0f}}});

        poly.setRadius(sqrtf(Math::square(init._size)));
    }
    else if(init.type == ObjectType::Capsule)
    {
        momentInertia = 1.0f*init._mass*Math::square(init._size);

        poly = ConcavePolygonWrap({
                                  sf::Vertex{{0.0f, -init._size.y/2.0f}},
                                  sf::Vertex{{0.0f, init._size.y/2.0f}}
                                  });

        poly.setRadius(init._size.x);
    }

    poly.setPosition(init._position);
    poly.setRotation(init._rotation);

    if(init._isStatic)
    {
        setMass(1e+15);
        setMomentInertia(1e+15);
    }
}

Polygon::~Polygon() {}

ObjectType Polygon::type() const { return MY_TYPE; }


sf::Rect<float > Polygon::getBoundingBox()
{
    float maxX = farthestPointInDir({1.0f, 0.0f}).position.x;
    float maxY = farthestPointInDir({0.0f, 1.0f}).position.y;
    float minX = farthestPointInDir({-1.0f, 0.0f}).position.x;
    float minY = farthestPointInDir({0.0f, -1.0f}).position.y;

    sf::Rect<float > rect{minX, minY, maxX-minX, maxY-minY};
    return rect;
}



sf::Vertex Polygon::farthestPointInDir(sf::Vector2f direction)
{
    std::vector<sf::Vertex > obbVerts = constructVerts();
    sf::Vertex result;

    float maxProj = -1e15;
    for(unsigned int i=0; i<obbVerts.size(); ++i)
    {
        float currProj = Math::dot(obbVerts[i].position, direction);
        if(currProj > maxProj)
        {
            maxProj = currProj;
            result = obbVerts[i];
        }
    }

    return result;
}

float Polygon::getMinSize()
{
    sf::Rect<float > boundingBox = getBoundingBox();

    return boundingBox.height < boundingBox.width ?
            boundingBox.height : boundingBox.width;
}

std::vector<sf::Vertex > Polygon::constructVerts()
{
    std::vector<sf::Vertex > verts = poly.getVertices(false);

    for(int i=0; i<verts.size(); ++i)
    {
        verts[i].position = Math::rotate(verts[i].position, rotAngle);
        verts[i].position += position;
    }

    return verts;
}

void Polygon::getConvexBreakdown(std::vector<Polygon > & returnArr)
{

}
