#ifndef INTEGRATORS_H
#define INTEGRATORS_H
#include <SFML/Graphics.hpp>>

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
    sf::Vector2f eulerMethod(sf::Vector2f xvec, float dt, float M, float G=1);
    sf::Vector2f semImpEulerMethod(sf::Vector2f xvec, float dt, float M, float G=1);
    sf::Vector2f RK4Method1stODE(sf::Vector2f xvec, float dt, float M, float G=1);
    std::vector<sf::Vector2f> RK4Method2ndODE(sf::Vector2f xvec, float dt, float M, float G=1);
    std::vector<sf::Vector2f> verletMethod(sf::Vector2f xvec, sf::Vector2f velVec, float dt, float M, float G=1);
}

#endif // INTEGRATORS_H
