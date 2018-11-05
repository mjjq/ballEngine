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

#include "../../headers/classOBB.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/integrators.h"
#include "../../headers/stringConversion.h"

/**
    Construct the ball.

    @param radius The radius of the ball.
    @param mass The mass of the ball.
    @param initPos The initial position of the ball.
    @param initVel The initial velocity of the ball.
*/
OBB::OBB(sf::Vector2f _size,
         float _mass,
         sf::Vector2f _initPos,
         sf::Vector2f _initVel,
         float _rotation,
         float _rotRate) :
PhysicsObject(_initPos, _initVel, _mass),
         size{_size}
{
    if(size.x < 0.0f)
        size.x = -size.x;
    if(size.y < 0.0f)
        size.y = -size.y;
    density = _mass/(size.x*size.y);
    centreOfMass = {0.0f, 0.0f}; //sf::Vector2f{size.x/2.0f, size.y/2.0f};

    rotAngle = _rotation;
    rotRate = _rotRate;

    momentInertia = _mass*(sfVectorMath::square(size))/12.0f;
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
    sf::VertexArray obbVerts(sf::Points, 4);

    obbVerts[0].position = sf::Vector2f(-size.x/2.0f, -size.y/2.0f);
    obbVerts[1].position = sf::Vector2f(size.x/2.0f, -size.y/2.0f);
    obbVerts[2].position = sf::Vector2f(-size.x/2.0f, size.y/2.0f);
    obbVerts[3].position = sf::Vector2f(size.x/2.0f, size.y/2.0f);

    for(unsigned int i=0; i<4; ++i)
    {
        obbVerts[i].position = sfVectorMath::rotate(obbVerts[i].position,
                                                    rotAngle);
        obbVerts[i].position += position;
    }

    return obbVerts.getBounds();
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
        obbVerts[i].position = sfVectorMath::rotate(obbVerts[i].position,
                                                    rotAngle);
        obbVerts[i].position += position;
    }

    return obbVerts;
}

float OBB::getRotAngle()
{
    return rotAngle;
}
