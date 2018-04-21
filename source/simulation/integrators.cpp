#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "../../headers/integrators.h"
#include "../../headers/forces.h"

sf::Vector2f integrators::eulerMethod(sf::Vector2f xvec, float dt, float M, float G)
{
    return dt*forces::newtonForce(xvec, M, G);
}

sf::Vector2f integrators::semImpEulerMethod(sf::Vector2f xvec, float dt, float M, float G)
{
    return dt*forces::newtonForce(xvec, M, G);
}

sf::Vector2f integrators::RK4Method1stODE(sf::Vector2f xvec, float dt, float M, float G)
{
    sf::Vector2f k1 = forces::newtonForce(xvec, M, G);
    sf::Vector2f k2 = forces::newtonForce(xvec + 0.5f*dt*k1, M, G);
    sf::Vector2f k3 = forces::newtonForce(xvec + 0.5f*dt*k2, M, G);
    sf::Vector2f k4 = forces::newtonForce(xvec + dt*k3, M, G);

    return (dt/6.0f)*(k1+2.0f*k2+2.0f*k3+k4);
}

std::vector<sf::Vector2f> integrators::RK4Method2ndODE(sf::Vector2f xvec, float dt, float M, float G)
{
    sf::Vector2f k1 = forces::newtonForce(xvec, M, G);
    sf::Vector2f k2 = forces::newtonForce(xvec + 0.5f*dt*k1, M, G);
    sf::Vector2f k3 = forces::newtonForce(xvec + 0.5f*dt*k2, M, G);
    sf::Vector2f k4 = forces::newtonForce(xvec + dt*k3, M, G);

    //sf::Vector2f k1 = forces::newtonForce(xvec, M, G);
    //sf::Vector2f k2 = forces::newtonForce(xvec + 0.5f*dt*k1, M, G);
    //sf::Vector2f k3 = forces::newtonForce(xvec + 0.5f*dt*k2, M, G);

    return {(dt/6.0f)*(k1+2.0f*k2+2.0f*k3+k4), (dt/6.0f)*(k1 + k2 + k3)};
}

std::vector<sf::Vector2f> integrators::verletMethod(sf::Vector2f xvec, sf::Vector2f velVec, float dt, float M, float G)
{
    sf::Vector2f newXVec = (velVec + 0.5f*dt*forces::newtonForce(xvec, M, G));
    sf::Vector2f dv = 0.5f*(forces::newtonForce(xvec+newXVec*dt, M, G) + forces::newtonForce(xvec, M, G));

    return {dv*dt, newXVec-velVec};
}

