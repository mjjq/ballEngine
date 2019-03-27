#ifndef FORCES_H
#define FORCES_H
#include <SFML/Graphics.hpp>

namespace forces
{
    sf::Vector2f newtonForce(sf::Vector2f vec, float M, float G=10);
}


#endif // FORCES_H
