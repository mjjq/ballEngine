#ifndef CLASS_LIGHT_H
#define CLASS_LIGHT_H

#include "SFML/Graphics.hpp"
#include "Observer.h"

struct LightProperties
{
    sf::Vector3f position;
    sf::Vector3f color = sf::Vector3f(1.0, 1.0, 1.0);
    float constant = 1.0f;
    float linear = 0.00002f;
    float quadratic = 0.000005f;
    float lightMax = 1.0f;
    float umbralRadius = 1.0f;
};

struct Ray
{
public:
    Ray(sf::Vector2f const & _pos,
        sf::Vector2f const & _dir,
        float const & _maxT = 1e+15f) :
        pos{_pos}, dir{_dir}, maxT{_maxT} {}
    Ray() {}

    sf::Vector2f pos;
    sf::Vector2f dir;
    float maxT;
};

class LightSource : public Component
{
public:
    static Subject renderSubject;
    LightProperties lightProperties;

    sf::Vector3f position;
    float effectiveRadius = 300.0f;

    LightSource(LightProperties properties);
    ~LightSource();

    void calcEffectiveRadius(float attFactor);
};

#endif // CLASS_LIGHT_H
