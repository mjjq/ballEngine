#ifndef INTEGRATORS_H
#define INTEGRATORS_H
#include <SFML/Graphics.hpp>

typedef std::pair<sf::Vector2f, sf::Vector2f> sfPair;

enum class Integrators
{
    INTEG_EULER,
    INTEG_SEM_IMP_EULER,
    INTEG_RK4,
    INTEG_VERLET,
    LAST,
};

namespace integrators
{
    sfPair eulerMethod(sf::Vector2f xvec, float dt, float M, float G=1);
    sfPair semImpEulerMethod(sf::Vector2f xvec, float dt, float M, float G);
    sf::Vector2f RK4Method1stODE(sf::Vector2f xvec, float dt, float M, float G=1);
    sfPair RK4Method2ndODE(sf::Vector2f xvec, sf::Vector2f thisVel, sf::Vector2f otherVel, float dt, float M, float G);
    //sf::Vector2f verletMethod(sf::Vector2f xvec, sf::Vector2f &velVec, float dt, float M, float G=1);
    sfPair verletMethod(sf::Vector2f &xSep, sf::Vector2f &thisVel, sf::Vector2f &otherVel, float &dt, float M, float G);

    sfPair eulerMethod(float dt, sf::Vector2f g);
    sfPair semImpEulerMethod(float dt, sf::Vector2f g);
    sfPair RK4Method2ndODE(float dt, sf::Vector2f g);
    //sf::Vector2f verletMethod(sf::Vector2f xvec, sf::Vector2f &velVec, float dt, float M, float G=1);
    sfPair verletMethod(float dt, sf::Vector2f g);

    std::string returnIntTypeStr(const Integrators &value);
}

#endif // INTEGRATORS_H
