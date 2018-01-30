#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "classes.h"
#include "sfVectorMath.h"

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

float Ball::newtonForce(float x, float x_0, float r, float G, float M)
{
    float dv = -G*M*(x-x_0)/pow(r,3);
    return dv;
}


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

float Ball::timeToCollision(Ball &otherBall)
{
    using namespace sfVectorMath;

    sf::Vector2f relPos = getPosition() - otherBall.getPosition();
    sf::Vector2f relVel = getVelocity() - otherBall.getVelocity();
    float radSum = getRadius() + otherBall.getRadius();

    if(dot(relPos,relPos) < pow(radSum, 2))
        return std::numeric_limits<float>::quiet_NaN();

    float discriminant = pow(dot(relPos,relVel), 2) - dot(relVel,relVel)*(dot(relPos,relPos) - pow(radSum, 2));

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

    setToCollided();
    otherBall.setToCollided();
}


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

void Ball::updatePosition(float dt)
{
    //sf::Vector2f currPos = getPosition();
    setPosition(getPosition()+velocity*dt);
}

float Ball::getMass()
{
    return mass;
}

float Ball::getRadius()
{
    return radius;
}

sf::Vector2f Ball::getVelocity()
{
    return velocity;
}

void Ball::setVelocity(sf::Vector2f vel)
{
    velocity = vel;
}

void Ball::setToCollided()
{
    collidedThisFrame = true;
}

void Ball::resetToCollided()
{
    collidedThisFrame = false;
}

bool Ball::getHasCollided()
{
    return collidedThisFrame;
}

float Ball::getKE()
{
    return 0.5*getMass()*sfVectorMath::dot(getVelocity(),getVelocity());
}

float Ball::getDistance(Ball &otherBall)
{
    sf::Vector2f relPos = getPosition() - otherBall.getPosition();
    return pow(sfVectorMath::dot(relPos,relPos),0.5);
}

float Ball::getSpeed()
{
    return pow(sfVectorMath::dot(getVelocity(),getVelocity()),0.5);
}

float Ball::getRelSpeed(Ball &otherBall)
{
    sf::Vector2f relVelocity = getVelocity() - otherBall.getVelocity();
    return pow(sfVectorMath::dot(relVelocity,relVelocity),0.5);
}
