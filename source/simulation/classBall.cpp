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

/**
    Returns change in velocity of a Lennard-Jones based force. This function
    must be called twice; once for each orthogonal co-ordinate x and y.

    @param x The current position of the particle/ball.
    @param x_0 The current position of the other particle/ball to interact with.
    @param r The distance between the current and other particle/ball.
    @param m The mass of the particle/ball.

    @return Infinitesimal change in velocity.
*/
float Ball::lenJonesForce(float x, float x_0, float r, float m)
{
    float energy = 0.0000001;
    float r_0 = pow(2.0,1/6)*getRadius();
    float dv = 24*energy*(2*pow(r_0,12)/pow(r,14) - pow(r_0,6)/pow(r,8))*(x-x_0)/m;
    return dv;
}

float Ball::exptCollForce(float x, float x_0, float r, float m)
{
    float energy = 0.0000001;
    float r_0 = 0.998*getRadius();
    float dv = (x-x_0)*pow(r-r_0,-19)/m;
    return dv;
}


/**
    Returns change in velocity of a gravitational force. This function must be
    called twice; once for each orthogonal co-ordinate x and y.

    @param x The current position of the particle/ball.
    @param x_0 The current position of the other particle/ball to interact with.
    @param r The distance between the current and other particle/ball.
    @param G The gravitational constant.
    @param M The mass of the OTHER particle/ball.

    @return Infinitesimal change in velocity.
*/
float Ball::newtonForce(float x, float x_0, float r, float G, float M)
{
    float dv = -G*M*(x-x_0)/pow(r,3);
    return dv;
}

/**
    Construct the ball.

    @param radius The radius of the ball.
    @param mass The mass of the ball.
    @param initPos The initial position of the ball.
    @param initVel The initial velocity of the ball.
*/
Ball::Ball(float radius, float mass, sf::Vector2f initPos, sf::Vector2f initVel) :
sf::CircleShape(radius), mass(mass), velocity{initVel}, radius(radius)
{
    setPosition(initPos);
    setOrigin(radius,radius);
    if(mass>0)
        setFillColor(sf::Color::Green);
    else
        setFillColor(sf::Color::Red);
}


/**
    Calculate the time to collision with another ball.

    @param &otherBall Reference to the other ball.

    @return Time to collision.
*/
float Ball::timeToCollision(Ball &otherBall)
{
    using namespace sfVectorMath;

    sf::Vector2f relPos = getPosition() - otherBall.getPosition();
    float radSum = getRadius() + otherBall.getRadius();

    if(dot(relPos,relPos) < pow(radSum, 2))
        return std::numeric_limits<float>::quiet_NaN();

    sf::Vector2f relVel = getVelocity() - otherBall.getVelocity();
    float discriminant = pow(dot(relPos,relVel), 2) -
                         dot(relVel,relVel)*(dot(relPos,relPos) - pow(radSum, 2));

    if(discriminant < 0)
        return std::numeric_limits<float>::quiet_NaN();

    float root1 = -(dot(relPos,relVel) + pow(discriminant,0.5))/dot(relVel,relVel);
    float root2 = -(dot(relPos,relVel) - pow(discriminant,0.5))/dot(relVel,relVel);

    if(root1 < 0 && root2 < 0)
        return std::numeric_limits<float>::quiet_NaN();
    else if(root1 < 0 && root2 > 0)
        return root2;
    else if(root2 < 0 && root1 > 0)
        return root1;
    else
        return (root2<root1)?root2:root1;
}


/**
    Checks if the ball is about to intersect the world boundary and executes a
    damped collision.

    @param worldSizeX The x-component size of the simulation world.
    @param worldSizeY The y-component size of the simulation world.

    @return Void.
*/
void Ball::checkForBounce(int worldSizeX, int worldSizeY)
{
    sf::Vector2f shapePos = getPosition();
    float shapeRadius = getRadius();

    if(((shapePos.x+shapeRadius >= worldSizeX) && (velocity.x>=0))
    || ((shapePos.x-shapeRadius <= 0  && (velocity.x<=0))))
        velocity.x = -velocity.x*dampingFactor;
    if(((shapePos.y+shapeRadius >= worldSizeY) && (velocity.y>=0))
    || ((shapePos.y-shapeRadius <= 0  && (velocity.y<=0))))
        velocity.y = -velocity.y*dampingFactor;
}

/**
    Executes a collision with another ball.

    @param &otherBall The other ball to collide with.

    @return Void.
*/
void Ball::ballCollision(Ball &otherBall)
{
    using namespace sfVectorMath;

    sf::Vector2f relPos = getPosition() - otherBall.getPosition();
    sf::Vector2f rhat = norm(relPos);

    //if(getDistance(otherBall) < 0.8*(getRadius() + otherBall.getRadius()))
    //    std::cout << "Overlapping balls at: " << getPosition().x << "\n";


    sf::Vector2f v1 = getVelocity();
    sf::Vector2f v2 = otherBall.getVelocity();
    float m1 = getMass();
    float m2 = otherBall.getMass();

    sf::Vector2f v1parallel = rhat*dot(v1,rhat);
    sf::Vector2f v2parallel = rhat*dot(v2,rhat);
    sf::Vector2f vRelParFrame = v1parallel - v2parallel;
    sf::Vector2f v1PrimeFrame = vRelParFrame*(m1-m2)/(m1+m2);
    sf::Vector2f v2PrimeFrame = vRelParFrame*(2*m1)/(m1+m2);
    sf::Vector2f v1Perp = v1PrimeFrame + v2parallel;

    setVelocity(v1 - v1parallel + v1Perp);
    sf::Vector2f v2final = v2 + v2PrimeFrame;
    otherBall.setVelocity(v2final);

    //setToCollided();
    //otherBall.setToCollided();
}


/**
    Updates the velocity of the current ball by calculating forces on the ball.

    @param dt The simulation timestep.
    @param &otherBall The other ball to interact with.

    @return Void.
*/
void Ball::updateVelocity(float dt, Ball &otherBall)
{
    float x_0 = otherBall.getPosition().x;
    float y_0 = otherBall.getPosition().y;
    float x = getPosition().x;
    float y = getPosition().y;
    float r = sqrt(pow((x-x_0),2) + pow((y-y_0),2));
    float m = getMass();

    if(r > (otherBall.getRadius()+getRadius()))
    {
        float G = 5;
        float M = otherBall.getMass();
        velocity.x += newtonForce(x,x_0,r,G,M)*dt;
        velocity.y += newtonForce(y,y_0,r,G,M)*dt;
    }

}


/**
    Applies an external force in the chosen direction.

    @param force The directional force to apply.
    @param dt The simulation timestep.
*/
void Ball::applyExternalImpulse(sf::Vector2f force, float dt)
{
    velocity += force*dt/getMass();
}


/**
    Updates the ball position based on the current velocity.

    @param dt The simulation timestep.

    @return Void.
*/
void Ball::updatePosition(float dt)
{
    //sf::Vector2f currPos = getPosition();
    setPosition(getPosition()+velocity*dt);
}

/**
    Get the current mass of the ball.

    @return The mass of the ball.
*/
float Ball::getMass()
{
    return mass;
}


/**
    Get the current radius of the ball.

    @return The radius of the ball.
*/
float Ball::getRadius()
{
    return radius;
}

/**
    Get the current velocity of the ball.

    @return The velocity vector of the ball.
*/
sf::Vector2f Ball::getVelocity()
{
    return velocity;
}

/**
    Set the current velocity of the ball.

    @param vel The velocity vector to set the ball to.

    @return Void.
*/
void Ball::setVelocity(sf::Vector2f vel)
{
    velocity = vel;
}


/**
    Sets the ball collision state to true.

    @return Void.
*/
void Ball::setToCollided()
{
    collidedThisFrame = true;
}

/**
    Sets the ball collision state to false.

    @return Void.
*/
void Ball::resetToCollided()
{
    collidedThisFrame = false;
}


/**
    Get the collision state of the ball.

    @return The collision state of the ball.
*/
bool Ball::getHasCollided()
{
    return collidedThisFrame;
}

/**
    Get the kinetic energy of the ball.

    @return The kinetic energy of the ball.
*/
float Ball::getKE()
{
    return 0.5*getMass()*sfVectorMath::dot(getVelocity(),getVelocity());
}


/**
    Get the momentum vector of the ball.

    @return The momentum vector of the ball.
*/
sf::Vector2f Ball::getMomentum()
{
    return getMass()*velocity;
}


/**
    Get the distance between this ball and another ball.

    @param &otherBall The other ball to calculate distance to.

    @return The distance between this ball and the other ball.
*/
float Ball::getDistance(Ball &otherBall)
{
    sf::Vector2f relPos = getPosition() - otherBall.getPosition();
    return pow(sfVectorMath::dot(relPos,relPos),0.5);
}


/**
    Get the current speed of the ball.

    @return The current speed of the ball.
*/
float Ball::getSpeed()
{
    return pow(sfVectorMath::dot(getVelocity(),getVelocity()),0.5);
}


/**
    Get the relative speed between this ball and another ball.

    @param &otherBall The other ball to calculate relative speed to.

    @return Relative speed between this ball and the other ball.
*/
float Ball::getRelSpeed(Ball &otherBall)
{
    sf::Vector2f relVelocity = getVelocity() - otherBall.getVelocity();
    return pow(sfVectorMath::dot(relVelocity,relVelocity),0.5);
}


/**
    Stores the current position of the ball to a std::vector containing the
    "history" of the ball positions. Also erases the oldest entry in the
    history vector given by positionSize.

    @return Void.
*/
void Ball::sampleNextPosition()
{
    int positionSize = 10;
    previousPositions.push_back(getPosition());
    if(previousPositions.size()>positionSize)
    {
        int eraseUpperLimit = previousPositions.size() - positionSize;
        previousPositions.erase(previousPositions.begin(),
                                previousPositions.begin()+eraseUpperLimit);
        //previousPositions.pop_front();
    }
}


/**
    Stores the current position of the ball to a std::deque containing the
    "history" of the ball positions.

    @return Void.
*/
void Ball::sampleCurrentPosition()
{
    if(previousPositions.size()>0)
        previousPositions.back() = getPosition();
}


/**
    Returns the std::deque list of previous positions of the ball.

    @return List of previous positions of the ball.
*/
std::deque<sf::Vector2f>& Ball::getPreviousPositions()
{
    return previousPositions;
}

/**
    Returns the state of whether the ball samples previous positions.

    @return The state of whether the ball samples previous positions.
*/
bool Ball::getSamplePrevPosBool()
{
    return samplePreviousPositions;
}
