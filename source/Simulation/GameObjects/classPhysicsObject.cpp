#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "classPhysicsObject.h"
#include "sfVectorMath.h"
#include "integrators.h"
#include "stringConversion.h"

PhysicsObject::PhysicsObject(ObjectProperties init) :
                  coefRestitution{init._coefRest},
                  coefFriction{init._coefFric},
                  position{init._position},
                  nStepPosition{init._position},
                  cStepVelocity{init._velocity},
                  rotAngle{init._rotation},
                  rotRate{init._rotRate},
                  mass{init._mass}
                  {}

PhysicsObject::~PhysicsObject() {}



sf::Vector2f PhysicsObject::getPosition() {return position;}
void PhysicsObject::setPosition(sf::Vector2f newPosition) {position = newPosition;}

/**
    Applies an external force in the chosen direction.

    @param force The directional force to apply.
    @param dt The simulation timestep.
*/


void PhysicsObject::applyExternalImpulse(sf::Vector2f force, float dt)
{
    nStepVelocity += force*dt/getMass();
    //nStepVelocity = cStepVelocity;
}


/**
    Updates the ball position based on the current velocity.

    @param dt The simulation timestep.

    @return Void.
*/
void PhysicsObject::updatePosition(float dt)
{
    //std::cout << dt << "\n";
    //printAllVelocities();
    //sf::Vector2f currPos = getPosition();
    pStepPosition = getPosition();
    setPosition(getPosition()+(cStepVelocity+cStepModVelocity)*dt);
    rotAngle += rotRate*dt*180.0f/sfVectorMath::PI;
    //std::cout << rotRate << "\n";
    //std::cout << "Current: " << cStepVelocity << "\n";
    //std::cout << "Next:    " << nStepVelocity << "\n";
    cStepVelocity = nStepVelocity;
    cStepModVelocity = {0,0};
}

/**
    Get the current mass of the ball.

    @return The mass of the ball.
*/
float PhysicsObject::getMass()
{
    return mass;
}

/**
    Set the current mass of the ball.

    @param _mass the mass to set the ball.

    @return Void.
*/
void PhysicsObject::setMass(float _mass)
{
    mass = _mass;
}


/**
    Get the current density of the ball.

    @return The density of the ball.
*/
float PhysicsObject::getDensity()
{
    return density;
}


/**
    Get the current radius of the ball.

    @return The radius of the ball.
*/
/*float PhysicsObject::getRadius()
{
    return radius;
}*/

/**
    Get the current velocity of the ball.

    @return The velocity vector of the ball.
*/
sf::Vector2f PhysicsObject::getVelocity()
{
    return cStepVelocity;
}

sf::Vector2f PhysicsObject::getnStepVelocity()
{
    return nStepVelocity;
}

void PhysicsObject::printAllVelocities()
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
void PhysicsObject::setVelocity(sf::Vector2f vel)
{
    cStepVelocity = vel;
    nStepVelocity = vel;
}

void PhysicsObject::addSolvedVelocity(sf::Vector2f cStep, sf::Vector2f nStep)
{
    cStepVelocity += cStep;
    nStepVelocity += nStep;
}


void PhysicsObject::addRotRate(float _rotRate)
{
    rotRate += _rotRate;
}


/**
    Get the kinetic energy of the ball.

    @return The kinetic energy of the ball.
*/
float PhysicsObject::getKE()
{
    return 0.5*getMass()*sfVectorMath::dot(getVelocity(),getVelocity());
}



/**
    Get the momentum vector of the ball.

    @return The momentum vector of the ball.
*/
sf::Vector2f PhysicsObject::getMomentum()
{
    return getMass()*cStepVelocity;
}




/**
    Get the current speed of the ball.

    @return The current speed of the ball.
*/
float PhysicsObject::getSpeed()
{
    return pow(sfVectorMath::dot(getVelocity(),getVelocity()),0.5);
}


/**
    Get the relative speed between this ball and another ball.

    @param &otherBall The other ball to calculate relative speed to.

    @return Relative speed between this ball and the other ball.
*/
float PhysicsObject::getRelSpeed(PhysicsObject* otherObj)
{
    sf::Vector2f relVelocity = getVelocity() - otherObj->getVelocity();
    return pow(sfVectorMath::dot(relVelocity,relVelocity),0.5);
}


/**
    Get the distance between this ball and another ball.

    @param &otherBall The other ball to calculate distance to.

    @return The distance between this ball and the other ball.
*/
float PhysicsObject::getDistance(PhysicsObject* otherObj)
{
    sf::Vector2f relPos = getPosition() - otherObj->getPosition();
    return pow(sfVectorMath::dot(relPos,relPos),0.5);
}



float PhysicsObject::getGPE(PhysicsObject* otherObj)
{
    return -getMass()*otherObj->getMass()/getDistance(otherObj);
}



/**
    Stores the current position of the ball to a std::vector containing the
    "history" of the ball positions. Also erases the oldest entry in the
    history vector given by positionSize.

    @return Void.
*/
void PhysicsObject::sampleNextPosition()
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
void PhysicsObject::sampleCurrentPosition()
{
    if(previousPositions.size()>0)
        previousPositions.back() = getPosition();
}


/**
    Returns the std::deque list of previous positions of the ball.

    @return List of previous positions of the ball.
*/
std::deque<sf::Vector2f>& PhysicsObject::getPreviousPositions()
{
    return previousPositions;
}

sf::Vector2f PhysicsObject::getPreviousPosition()
{
    return pStepPosition;
}

/**
    Returns the state of whether the ball samples previous positions.

    @return The state of whether the ball samples previous positions.
*/
bool PhysicsObject::getSamplePrevPosBool()
{
    return samplePreviousPositions;
}

void PhysicsObject::setSamplePrevPosBool(bool value)
{
    if(samplePreviousPositions!=value)
        previousPositions.clear();
    samplePreviousPositions = value;
}

bool PhysicsObject::getIsPlayer()
{
    return isPlayer;
}

void PhysicsObject::setIsPlayer(bool value)
{
    isPlayer = value;
}

void PhysicsObject::incTimesCollided()
{
    ++numDynColls;
}

int PhysicsObject::getNumCollTimes()
{
    return numDynColls;
}


sf::Vector2f PhysicsObject::getCoM()
{
    return position + centreOfMass;
}

float PhysicsObject::getMomentInertia()
{
    return momentInertia;
}

void PhysicsObject::setMomentInertia(float i)
{
    momentInertia = i;
}

float PhysicsObject::getRotRate()
{
    return rotRate;
}

void PhysicsObject::setRotRate(float _rotRate)
{
    rotRate = _rotRate;
}

float PhysicsObject::getCoefRestitution()
{
    return coefRestitution;
}

float PhysicsObject::getCoefFriction()
{
    return coefFriction;
}
