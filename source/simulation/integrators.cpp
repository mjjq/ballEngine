#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "../../headers/integrators.h"
#include "../../headers/forces.h"

sfPair integrators::eulerMethod(sf::Vector2f xvec, float dt, float M, float G)
{
    return std::make_pair( sf::Vector2f{0,0}, dt*forces::newtonForce(xvec, M, G) );
}

sfPair integrators::semImpEulerMethod(sf::Vector2f xvec, float dt, float M, float G)
{
    return std::make_pair( dt*forces::newtonForce(xvec, M, G), dt*forces::newtonForce(xvec, M, G) );
}

sf::Vector2f integrators::RK4Method1stODE(sf::Vector2f xvec, float dt, float M, float G)
{
    sf::Vector2f k1 = forces::newtonForce(xvec, M, G);
    sf::Vector2f k2 = forces::newtonForce(xvec + 0.5f*dt*k1, M, G);
    sf::Vector2f k3 = forces::newtonForce(xvec + 0.5f*dt*k2, M, G);
    sf::Vector2f k4 = forces::newtonForce(xvec + dt*k3, M, G);

    return (dt/6.0f)*(k1+2.0f*k2+2.0f*k3+k4);
}

sfPair integrators::RK4Method2ndODE(sf::Vector2f xvec, sf::Vector2f thisVel, sf::Vector2f otherVel, float dt, float M, float G)
{
    sf::Vector2f kv1 = forces::newtonForce(xvec, M, G);
    sf::Vector2f kr1 = thisVel-otherVel;
    sf::Vector2f kv2 = forces::newtonForce(xvec + 0.5f*dt*kr1, M, G);
    sf::Vector2f kr2 = thisVel - otherVel + 0.5f*dt*kv1;
    sf::Vector2f kv3 = forces::newtonForce(xvec + 0.5f*dt*kr2, M, G);
    sf::Vector2f kr3 = thisVel - otherVel + 0.5f*dt*kv2;
    sf::Vector2f kv4 = forces::newtonForce(xvec + dt*kr3, M, G);
    //sf::Vector2f kr4 = thisVel - otherVel + dt*kv3;

    //sf::Vector2f k1 = forces::newtonForce(xvec, M, G);
    //sf::Vector2f k2 = forces::newtonForce(xvec + 0.5f*dt*k1, M, G);
    //sf::Vector2f k3 = forces::newtonForce(xvec + 0.5f*dt*k2, M, G);

    return std::make_pair( (dt/6.0f)*(kv1 + kv2 + kv3), (dt/6.0f)*(kv1+2.0f*kv2+2.0f*kv3+kv4) );
}

/*sf::Vector2f integrators::verletMethod(sf::Vector2f xvec, sf::Vector2f &velVec, float dt, float M, float G)
{
    sf::Vector2f newVelVec = (velVec + 0.5f*dt*forces::newtonForce(xvec, M, G));
    sf::Vector2f dv = 0.5f*(forces::newtonForce(xvec+newVelVec*dt, M, G) + forces::newtonForce(xvec, M, G));

    velVec = newVelVec;

    return dv*dt;
}*/

sfPair integrators::verletMethod(sf::Vector2f &xSep, sf::Vector2f &thisVel, sf::Vector2f &otherVel, float &dt, float M, float G)
{
    sf::Vector2f dv = dt*forces::newtonForce(xSep + 0.5f*dt*(thisVel - otherVel), M, G);
    sf::Vector2f dx = 0.5f*(dv);

    return std::make_pair(dx, dv);
}

std::string integrators::returnIntTypeStr(const Integrators &value)
{
    switch(value)
    {
        case(Integrators::INTEG_EULER):
            return std::string("Euler");
        case(Integrators::INTEG_SEM_IMP_EULER):
            return std::string("Semi Imp Euler");
        case(Integrators::INTEG_RK4):
            return std::string("RK4");
        case(Integrators::INTEG_VERLET):
            return std::string("Vel Verlet");
        case(Integrators::LAST):
        default:
            return std::string("Error, invalid integrator");
    }
}

