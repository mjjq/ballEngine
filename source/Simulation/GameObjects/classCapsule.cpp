/**
    @class classCapsule
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

#include "classCapsule.h"
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
Capsule::Capsule(ObjectProperties init) :
PhysicsObject(init)
{
    centrePosition = sf::Vector2f{0.0f, 0.0f};

    vertices.push_back({{0.0f, -init._size.y/2.0f}});
    vertices.push_back({{0.0f, init._size.y/2.0f}});
    radius = init._size.x;
    length = init._size.y;

    centreOfMass = {0.0f, 0.0f}; //sf::Vector2f{size.x/2.0f, size.y/2.0f};

    for(sf::Vertex &vert : vertices)
    {
        momentInertia += sfVectorMath::square(vert.position - centreOfMass);
    }
    momentInertia = momentInertia*init._mass/vertices.size();

    //rotAngle = init._rotation;
    //rotRate = init._rotRate;



    genBoundingOBB();
}

Capsule::~Capsule() {}

ObjectType Capsule::type() const { return MY_TYPE; }

void Capsule::draw(sf::RenderWindow &_window)
{
    sf::CircleShape circ1(radius);
    circ1.setOrigin({radius, radius});
    sf::CircleShape circ2 = circ1;
    sf::RectangleShape rect({2.0f*radius, length});
    rect.setOrigin({radius, length/2.0f});
    rect.rotate(rotAngle);

    circ1.setPosition(position + sfVectorMath::rotate(vertices[0].position, rotAngle));
    circ2.setPosition(position + sfVectorMath::rotate(vertices[1].position, rotAngle));
    rect.setPosition(position);

    //_window.draw(temp.data(), numVerts, sf::Lines);
    _window.draw(circ1);
    _window.draw(circ2);
    _window.draw(rect);
}

float Capsule::getMinSize()
{
    return radius;
}


sf::Rect<float > Capsule::getGlobalBounds()
{
    return boundingOBB;
}

sf::Rect<float > Capsule::getBoundingBox()
{
    sf::VertexArray CapsuleVerts(sf::Points, 4);

    CapsuleVerts[0] = farthestPointInDir({1.0f, 0.0f});
    CapsuleVerts[1] = farthestPointInDir({0.0f, 1.0f});
    CapsuleVerts[2] = farthestPointInDir({-1.0f, 0.0f});
    CapsuleVerts[3] = farthestPointInDir({0.0f, -1.0f});

    return CapsuleVerts.getBounds();
}

std::vector<sf::Vertex > Capsule::constructVerts()
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

std::vector<sf::Vertex > Capsule::constructLocalVerts()
{
    return vertices;
}

float Capsule::getRotAngle()
{
    return rotAngle;
}

void Capsule::genBoundingOBB()
{
    boundingOBB = getBoundingBox();
}

sf::Vertex Capsule::farthestPointInDir(sf::Vector2f direction)
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

float Capsule::getRadius()
{
    return radius;
}
