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

#include "../../headers/classAABB.h"
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
AABB::AABB(sf::Vector2f _size, float _mass, sf::Vector2f _initPos, sf::Vector2f _initVel) :
PhysicsObject(_initPos, _initVel, _mass), size{_size}
{
    density = _mass/(size.x*size.y);
}

AABB::~AABB() {}

ObjectType AABB::type() const { return MY_TYPE; }

void AABB::draw(sf::RenderWindow &_window)
{
    sf::RectangleShape drawable;
    drawable.setSize(size);
    drawable.setPosition(position);
    //nStepPosition = initPos;

    drawable.setFillColor(sf::Color{44,44,44,255});

    drawable.setOutlineThickness(-1);
    _window.draw(drawable);
    //std::cout << drawable.getPosition() << "\n";
}

float AABB::getMinSize()
{
    return ((size.x < size.y) ? size.x : size.y);
}

sf::Rect<float > AABB::getGlobalBounds()
{
    return sf::Rect<float >{position.x, position.y, size.x, size.y};
}

