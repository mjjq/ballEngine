#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "../../headers/classDynamicObject.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/integrators.h"
#include "../../headers/stringConversion.h"

DynamicObject::DynamicObject(sf::Vector2f _position,
                  sf::Vector2f _velocity,
                  float _mass) : position{_position},
                  nStepPosition{_position},
                  cStepVelocity{_velocity},
                  mass{_mass}
                  {}

DynamicObject::~DynamicObject() {}



sf::Vector2f DynamicObject::getPosition() {return position;}
void DynamicObject::setPosition(sf::Vector2f newPosition) {position = newPosition;}

/**
    Applies an external force in the chosen direction.

    @param force The directional force to apply.
    @param dt The simulation timestep.
*/


void DynamicObject::applyExternalImpulse(sf::Vector2f force, float dt)
{
    nStepVelocity += force*dt/getMass();
    //nStepVelocity = cStepVelocity;
}


/**
    Updates the ball position based on the current velocity.

    @param dt The simulation timestep.

    @return Void.
*/
void DynamicObject::updatePosition(float dt)
{
    //std::cout << dt << "\n";
    //printAllVelocities();
    //sf::Vector2f currPos = getPosition();
    pStepPosition = getPosition();
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
float DynamicObject::getMass()
{
    return mass;
}

/**
    Set the current mass of the ball.

    @param _mass the mass to set the ball.

    @return Void.
*/
void DynamicObject::setMass(float _mass)
{
    mass = _mass;
}


/**
    Get the current density of the ball.

    @return The density of the ball.
*/
float DynamicObject::getDensity()
{
    return density;
}


/**
    Get the current radius of the ball.

    @return The radius of the ball.
*/
/*float DynamicObject::getRadius()
{
    return radius;
}*/

/**
    Get the current velocity of the ball.

    @return The velocity vector of the ball.
*/
sf::Vector2f DynamicObject::getVelocity()
{
    return cStepVelocity;
}

sf::Vector2f DynamicObject::getnStepVelocity()
{
    return nStepVelocity;
}

void DynamicObject::printAllVelocities()
{
    //std::cout << "cStep: " << cStepVelocity << "\n";
    //std::cout << "cStepMod: " << cStepModVelocity << "\n";
    //std::cout << "nStep: " << nStepVelocity << "\n\n";
}

/**
    Set the current velocity of the ball.

    @param vel The velocity vector to set the ball to.

    @return Void.
*/
void DynamicObject::setVelocity(sf::Vector2f vel)
{
    cStepVelocity = vel;
    nStepVelocity = vel;
}

void DynamicObject::addSolvedVelocity(sf::Vector2f cStep, sf::Vector2f nStep)
{
    cStepVelocity += cStep;
    nStepVelocity += nStep;
}



/**
    Get the kinetic energy of the ball.

    @return The kinetic energy of the ball.
*/
float DynamicObject::getKE()
{
    return 0.5*getMass()*sfVectorMath::dot(getVelocity(),getVelocity());
}



/**
    Get the momentum vector of the ball.

    @return The momentum vector of the ball.
*/
sf::Vector2f DynamicObject::getMomentum()
{
    return getMass()*cStepVelocity;
}




/**
    Get the current speed of the ball.

    @return The current speed of the ball.
*/
float DynamicObject::getSpeed()
{
    return pow(sfVectorMath::dot(getVelocity(),getVelocity()),0.5);
}





/**
    Stores the current position of the ball to a std::vector containing the
    "history" of the ball positions. Also erases the oldest entry in the
    history vector given by positionSize.

    @return Void.
*/
void DynamicObject::sampleNextPosition()
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
void DynamicObject::sampleCurrentPosition()
{
    if(previousPositions.size()>0)
        previousPositions.back() = getPosition();
}


/**
    Returns the std::deque list of previous positions of the ball.

    @return List of previous positions of the ball.
*/
std::deque<sf::Vector2f>& DynamicObject::getPreviousPositions()
{
    return previousPositions;
}

sf::Vector2f DynamicObject::getPreviousPosition()
{
    return pStepPosition;
}

/**
    Returns the state of whether the ball samples previous positions.

    @return The state of whether the ball samples previous positions.
*/
bool DynamicObject::getSamplePrevPosBool()
{
    return samplePreviousPositions;
}

void DynamicObject::setSamplePrevPosBool(bool value)
{
    if(samplePreviousPositions!=value)
        previousPositions.clear();
    samplePreviousPositions = value;
}

bool DynamicObject::getIsPlayer()
{
    return isPlayer;
}

void DynamicObject::setIsPlayer(bool value)
{
    isPlayer = value;
}

void DynamicObject::incTimesCollided()
{
    ++numDynColls;
}

int DynamicObject::getNumCollTimes()
{
    return numDynColls;
}
