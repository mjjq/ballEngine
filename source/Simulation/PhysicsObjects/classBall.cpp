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

#include "classBall.h"
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
Ball::Ball(ObjectProperties init) :
PhysicsObject(init)
{
    radius = sqrtf(init._size.x * init._size.x +
                   init._size.y * init._size.y);
    density = init._mass/(3.14159265359*radius*radius);
    centreOfMass = {0.0f, 0.0f};

    momentInertia = 0.5*init._mass*radius*radius;

    generateBoundingSphere();
}

Ball::~Ball() {}

ObjectType Ball::type() const { return MY_TYPE; }

void Ball::draw(sf::RenderWindow &_window)
{
    sf::CircleShape drawable(radius);
    drawable.setPosition(position);
    //nStepPosition = initPos;
    drawable.setOrigin(radius,radius);

    drawable.setFillColor(sf::Color{180,180,180,100});

    sf::Vertex line[] = {
        sf::Vertex{position},
        sf::Vertex{position + Math::rotate(sf::Vector2f{0.0f, radius}, rotAngle)}
    };
    //std::cout << getRotRate() << "\n";
    //drawable.setOutlineThickness(-1);
    _window.draw(drawable);
    _window.draw(line, 2, sf::Lines);
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

sf::Vertex Ball::farthestPointInDir(sf::Vector2f direction)
{
    sf::Vector2f vertPos = getPosition();
    //vertPos += getRadius() * Math::norm(direction);

    return sf::Vertex{vertPos};
}

sf::Rect<float > Ball::getBoundingBox()
{
    sf::Rect<float > rect{position.x-radius, position.y-radius,
                            2.0f*radius, 2.0f*radius};
    return rect;
}
