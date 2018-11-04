#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <cassert>

#include "../../headers/collisionDetection.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"

std::vector<sf::Vector2f > Collisions::getContactPoints(std::vector<sf::Vertex > &obj1,
                                                        std::vector<sf::Vertex > &obj2,
                                                        sf::Vector2f contactNormal)
{
    Edge edge1 = Collisions::getBestEdge(obj1, contactNormal);
    Edge edge2 = Collisions::getBestEdge(obj2, -contactNormal);

    bool flip = false;
    Edge refEdge;
    Edge incEdge;
    if(std::abs(sfVectorMath::dot(edge1.dir, contactNormal)) <=
       std::abs(sfVectorMath::dot(edge2.dir, contactNormal)))
    {
        refEdge = edge1;
        incEdge = edge2;
    }
    else
    {
        refEdge = edge2;
        incEdge = edge1;

        flip = true;
    }

    sf::Vector2f refDir = sfVectorMath::norm(refEdge.dir);

    float o1 = sfVectorMath::dot(refDir, refEdge.v1);

    ClippedPoints cp = Collisions::clip(incEdge.v1, incEdge.v2, refDir, o1);
    if(cp.size() < 2)
        return ClippedPoints{};

    float o2 = sfVectorMath::dot(refDir, refEdge.v2);
    cp = clip(cp[0], cp[1], -refDir, -o2);

    if(cp.size() < 2)
        return ClippedPoints{};

    sf::Vector2f refNorm = Collisions::orthogonal(refNorm);

    if(flip) refNorm = -refNorm;

    float max = sfVectorMath::dot(refNorm, refEdge.max);

    if(sfVectorMath::dot(refNorm, *cp.begin()) - max < 0.0f)
        cp.erase(cp.begin());
    if(sfVectorMath::dot(refNorm, *cp.end()) - max < 0.0f)
        cp.erase(cp.end());

    return cp;
}


Edge Collisions::getBestEdge(std::vector<sf::Vertex > &obj, sf::Vector2f normal)
{
    int index = 0;
    float max = -1e+15;

    int c = obj.size();
    for (int i = 0; i < c; ++i)
    {
      float projection = sfVectorMath::dot(normal, obj[i].position);
      if (projection > max)
      {
        max = projection;
        index = i;
      }
    }

    sf::Vector2f v = obj[index].position;
    sf::Vector2f v1 = obj[index+1].position;
    sf::Vector2f v0 = obj[index-1].position;

    sf::Vector2f l = sfVectorMath::norm(v - v1);
    sf::Vector2f r = sfVectorMath::norm(v - v0);

    if(sfVectorMath::dot(r, normal) <= sfVectorMath::dot(l, normal))
    {
        return Edge{v, v0, v0-v, v0};
    }
    else
        return Edge{v, v1, v-v1, v};

}


ClippedPoints Collisions::clip(sf::Vector2f v1,
                               sf::Vector2f v2,
                               sf::Vector2f normal,
                               float o)
{
    ClippedPoints cp;

    float d1 = sfVectorMath::dot(normal, v1) - o;
    float d2 = sfVectorMath::dot(normal, v2) - o;

    if(d1 >= 0.0f) cp.push_back(v1);
    if(d2 >= 0.0f) cp.push_back(v2);

    if(d1*d2 < 0.0f)
    {
        sf::Vector2f e = v2 - v1;
        float u = d1 / (d1-d2);

        e = u*e;
        e += v1;
        cp.push_back(e);
    }

    return cp;
}
