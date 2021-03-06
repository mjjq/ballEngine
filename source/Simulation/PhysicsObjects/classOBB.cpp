/**
    @class classOBB
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

#include "classOBB.h"
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
OBB::OBB(ObjectProperties init) :
PhysicsObject(init),
         size{init._size}
{
    if(size.x < 0.0f)
        size.x = -size.x;
    if(size.y < 0.0f)
        size.y = -size.y;
    density = init._mass/(size.x*size.y);
    centreOfMass = {0.0f, 0.0f}; //sf::Vector2f{size.x/2.0f, size.y/2.0f};

    rotAngle = init._rotation;
    rotRate = init._rotRate;

    momentInertia = init._mass*(Math::square(size))/12.0f;
}

OBB::~OBB() {}

ObjectType OBB::type() const { return MY_TYPE; }

void OBB::draw(sf::RenderWindow &_window)
{
    sf::RectangleShape drawable;
    drawable.setSize(size);
    drawable.setPosition(position);
    drawable.setOrigin(size.x/2.0f, size.y/2.0f);
    //nStepPosition = initPos;
    drawable.setRotation(rotAngle);
    drawable.setFillColor(sf::Color{180,180,180,100});

    //drawable.setOutlineThickness(-1);
    _window.draw(drawable);
    //std::cout << drawable.getPosition() << "\n";
}

float OBB::getMinSize()
{
    return ((size.x < size.y) ? size.x/2.0f : size.y/2.0f);
}


sf::Rect<float > OBB::getGlobalBounds()
{
    return sf::Rect<float >{position.x, position.y, size.x, size.y};
}

sf::Rect<float > OBB::getBoundingBox()
{
    float maxX = farthestPointInDir({1.0f, 0.0f}).position.x;
    float maxY = farthestPointInDir({0.0f, 1.0f}).position.y;
    float minX = farthestPointInDir({-1.0f, 0.0f}).position.x;
    float minY = farthestPointInDir({0.0f, -1.0f}).position.y;

    sf::Rect<float > rect{minX, minY, maxX-minX, maxY-minY};
    return rect;
}

std::vector<sf::Vertex > OBB::constructVerts()
{
    //make sure the vertices are counter-clockwise!
    std::vector<sf::Vertex > obbVerts;
    obbVerts.push_back(sf::Vertex(sf::Vector2f(-size.x/2.0f, -size.y/2.0f)));
    obbVerts.push_back(sf::Vertex(sf::Vector2f(size.x/2.0f, -size.y/2.0f)));
    obbVerts.push_back(sf::Vertex(sf::Vector2f(size.x/2.0f, size.y/2.0f)));
    obbVerts.push_back(sf::Vertex(sf::Vector2f(-size.x/2.0f, size.y/2.0f)));

    for(unsigned int i=0; i<4; ++i)
    {
        obbVerts[i].position = Math::rotate(obbVerts[i].position,
                                                    rotAngle);
        obbVerts[i].position += position;
    }

    return obbVerts;
}

float OBB::getRotAngle()
{
    return rotAngle;
}

sf::Vertex OBB::farthestPointInDir(sf::Vector2f direction)
{
    std::vector<sf::Vertex > obbVerts = constructVerts();
    sf::Vertex result;

    float maxProj = -1e15;
    for(unsigned int i=0; i<4; ++i)
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
