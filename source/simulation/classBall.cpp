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
    //float energy = 0.0000001;
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
Ball::Ball(float _radius, float _mass, sf::Vector2f _initPos, sf::Vector2f _initVel) :
sf::CircleShape(_radius), cStepVelocity{_initVel}, nStepPosition(_initPos), mass(_mass)
{
    setPosition(_initPos);
    //nStepPosition = initPos;
    setOrigin(_radius,_radius);
    if(mass>0)
        setFillColor(sf::Color::Green);
    else
        setFillColor(sf::Color::Red);
}





/**
    Applies an external force in the chosen direction.

    @param force The directional force to apply.
    @param dt The simulation timestep.
*/
void Ball::applyExternalImpulse(sf::Vector2f force, float dt)
{
    nStepVelocity += force*dt/getMass();
    //nStepVelocity = cStepVelocity;
}


/**
    Updates the ball position based on the current velocity.

    @param dt The simulation timestep.

    @return Void.
*/
void Ball::updatePosition(float dt)
{
    //sf::Vector2f currPos = getPosition();
    setPosition(getPosition()+(cStepVelocity+cStepModVelocity)*dt);
    //std::cout << "Current: " << cStepVelocity << "\n";
    //std::cout << "Next:    " << nStepVelocity << "\n";
    cStepVelocity = nStepVelocity;
    cStepModVelocity = {0,0};
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
/*float Ball::getRadius()
{
    return radius;
}*/

/**
    Get the current velocity of the ball.

    @return The velocity vector of the ball.
*/
sf::Vector2f Ball::getVelocity()
{
    return cStepVelocity;
}

/**
    Set the current velocity of the ball.

    @param vel The velocity vector to set the ball to.

    @return Void.
*/
void Ball::setVelocity(sf::Vector2f vel)
{
    cStepVelocity = vel;
    nStepVelocity = vel;
}

void Ball::addSolvedVelocity(sf::Vector2f &cStep, sf::Vector2f &nStep)
{
    cStepModVelocity += cStep;
    nStepVelocity += nStep;
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

float Ball::getGPE(Ball &otherBall)
{
    return -getMass()*otherBall.getMass()/getDistance(otherBall);
}


/**
    Get the momentum vector of the ball.

    @return The momentum vector of the ball.
*/
sf::Vector2f Ball::getMomentum()
{
    return getMass()*cStepVelocity;
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
    unsigned int positionSize = 10;
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

void Ball::setSamplePrevPosBool(bool value)
{
    if(samplePreviousPositions!=value)
        previousPositions.clear();
    samplePreviousPositions = value;
}

bool Ball::getIsPlayer()
{
    return isPlayer;
}

void Ball::setIsPlayer(bool value)
{
    isPlayer = value;
}
