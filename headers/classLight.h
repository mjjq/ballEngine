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
    float quadratic = 0.0001f;
    float lightMax = 1.0f;
};

struct Ray
{
public:
    Ray(sf::Vector2f const & _pos,
        sf::Vector2f const & _dir,
        float const & _maxT = 1e+15f) :
        pos{_pos}, dir{_dir}, maxT{_maxT} {}

    sf::Vector2f pos;
    sf::Vector2f dir;
    float maxT;
};

class LightSource : public Entity
{
public:
    static Subject renderSubject;
    LightProperties lightProperties;

    sf::Vector3f position;
    float effectiveRadius = 300.0f;

    LightSource(LightProperties properties);
    ~LightSource();

    sf::VertexArray shadowStencil(sf::Shape &shape);
    void calcEffectiveRadius(float attFactor);
};

#endif // CLASS_LIGHT_H
