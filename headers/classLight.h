#ifndef CLASS_LIGHT_H
#define CLASS_LIGHT_H

#include "SFML/Graphics.hpp"
#include "Observer.h"

struct LightProperties
{
    sf::Vector3f position;
    sf::Vector3f color = sf::Vector3f(1.0, 1.0, 1.0);
    float constant = 1.0f;
    float linear = 0.0005f;
    float quadratic = 0.0f;
};

class LightSource : public Entity
{
public:
    static Subject renderSubject;
    sf::Vector3f position;

    LightProperties lightProperties;

    LightSource(LightProperties properties);
    ~LightSource();
};

#endif // CLASS_LIGHT_H
