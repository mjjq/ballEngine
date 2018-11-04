/**
    @class classBall
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

#include "../../headers/classBall.h"
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
Ball::Ball(float _radius, float _mass, sf::Vector2f _initPos, sf::Vector2f _initVel) :
PhysicsObject(_initPos, _initVel, _mass), radius{_radius}
{
    density = _mass/(3.14159265359*_radius*_radius);
    centreOfMass = {0.0f, 0.0f};

    momentInertia = 0.5*_mass*_radius*_radius;
}

Ball::~Ball() {}

ObjectType Ball::type() const { return MY_TYPE; }

void Ball::draw(sf::RenderWindow &_window)
{
    sf::CircleShape drawable(radius);
    drawable.setPosition(position);
    //nStepPosition = initPos;
    drawable.setOrigin(radius,radius);

    drawable.setFillColor(sf::Color{180,180,180,255});

    //drawable.setOutlineThickness(-1);
    _window.draw(drawable);
    //std::cout << drawable.getPosition() << "\n";
}

float Ball::getRadius()
{
    return radius;
}

float Ball::getMinSize()
{
    return radius;
}

sf::Vector2f Ball::getCoM()
{
    return centreOfMass;
}
