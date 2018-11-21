/**
    @class classAABB
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

#include "classAABB.h"
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
AABB::AABB(ObjectProperties init) :
PhysicsObject(init), size{init._size}
{
    density = init._mass/(size.x*size.y);
    centreOfMass = sf::Vector2f{size.x/2.0f, size.y/2.0f};

    momentInertia = init._mass*(sfVectorMath::square(size))/12.0f;
}

AABB::~AABB() {}

ObjectType AABB::type() const { return MY_TYPE; }

void AABB::draw(sf::RenderWindow &_window)
{
    sf::RectangleShape drawable;
    drawable.setSize(size);
    drawable.setPosition(position);
    //nStepPosition = initPos;

    drawable.setFillColor(sf::Color{180,180,180,100});

    //drawable.setOutlineThickness(-1);
    _window.draw(drawable);
    //std::cout << drawable.getPosition() << "\n";
}

float AABB::getMinSize()
{
    return ((size.x < size.y) ? size.x/2.0f : size.y/2.0f);
}

/*sf::Vector2f AABB::getCoM()
{
    return centreOfMass;
}*/

sf::Rect<float > AABB::getGlobalBounds()
{
    return sf::Rect<float >{position.x, position.y, size.x, size.y};
}


std::vector<sf::Vertex > AABB::constructVerts()
{
    std::vector<sf::Vertex > obbVerts;
    obbVerts.push_back(sf::Vertex(sf::Vector2f(-size.x/2.0f, -size.y/2.0f)));
    obbVerts.push_back(sf::Vertex(sf::Vector2f(size.x/2.0f, -size.y/2.0f)));
    obbVerts.push_back(sf::Vertex(sf::Vector2f(size.x/2.0f, size.y/2.0f)));
    obbVerts.push_back(sf::Vertex(sf::Vector2f(-size.x/2.0f, size.y/2.0f)));

    for(unsigned int i=0; i<4; ++i)
    {
        obbVerts[i].position += position + centreOfMass;
    }

    return obbVerts;
}
