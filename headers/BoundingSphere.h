#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include <SFML/Graphics.hpp>
#include "Math.h"

struct BoundingSphere
{
    BoundingSphere() {}
    BoundingSphere(sf::Vector2f const & _position,
                   float const & _radius) : position{_position},
                   radius{_radius} {}
    sf::Vector2f position;
    float radius;

    bool intersects(BoundingSphere const & otherSphere) const
    {
        float distSquare = Math::square(position - otherSphere.position);
        float totalRadius = radius + otherSphere.radius;

        if(distSquare <= totalRadius*totalRadius)
            return true;

        return false;
    }
};

#endif // BOUNDING_SPHERE_H
