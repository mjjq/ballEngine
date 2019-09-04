#ifndef BOUNDING_SPHERE_H
#define BOUNDING_SPHERE_H

#include <SFML/Graphics.hpp>
#include "Math.h"
#include <cmath>

struct BoundingSphere
{
    sf::Vector2f position = {0.0f, 0.0f};
    float radius = 0.0f;

    BoundingSphere() {}
    BoundingSphere(sf::Vector2f const & _position,
                   float const & _radius) : position{_position},
                   radius{_radius} {}
    BoundingSphere(std::vector<sf::Vertex > const & verts)
    {
        float maxRadiusSq = 0.0f;
        for(int i=0; i<verts.size(); ++i)
        {
            float radSq = Math::square(verts[i].position);
            if(radSq > maxRadiusSq)
            {
                maxRadiusSq = radSq;
            }
        }

        radius = sqrt(maxRadiusSq);
        position = Math::average(verts);
    }

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
