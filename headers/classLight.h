#ifndef CLASS_LIGHT_H
#define CLASS_LIGHT_H

#include "SFML/Graphics.hpp"
#include "Observer.h"

class LightSource : public Entity
{
public:
    static Subject renderSubject;
    sf::Vector3f position;
    sf::Vector3f color = sf::Vector3f(1.0, 0.5, 1.0);

    LightSource(sf::Vector3f _position,
                sf::Vector3f _color);
    ~LightSource();
};

#endif // CLASS_LIGHT_H
