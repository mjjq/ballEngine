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

std::vector<sf::Vector2f > Collisions::edgesOf(std::vector<sf::Vertex > &vertices)
{
    std::vector<sf::Vector2f > edges;

    unsigned int num = vertices.size();
    for(unsigned int i=0; i<num; ++i)
    {
        sf::Vector2f newEdge;
        newEdge = vertices[(i+1)%num].position - vertices[i].position;
        edges.push_back(newEdge);
    }

    return edges;
}

sf::Vector2f Collisions::orthogonal(sf::Vector2f &v)
{
    //std::cout << -v.y << " " << v.x << "\n";
    return {v.y, -v.x};
}

std::pair<bool, sf::Vector2f > Collisions::isSeparatingAxis(sf::Vector2f &orthog,
                                                std::vector<sf::Vertex> &obj1,
                                                std::vector<sf::Vertex> &obj2)
{
    float min1 = 1e+15f;
    float max1 = -min1;
    float min2 = min1;
    float max2 = -min1;

    for(sf::Vertex &vert : obj1)
    {
        float projection = sfVectorMath::dot(vert.position, orthog);

        min1 = std::min(min1, projection);
        max1 = std::max(max1, projection);
    }

    for(sf::Vertex &vert : obj2)
    {
        float projection = sfVectorMath::dot(vert.position, orthog);

        min2 = std::min(min2, projection);
        max2 = std::max(max2, projection);
    }

    if(max1 >= min2 && max2 >= min1)
    {
        float d = std::min(max2 - min1, max1 - min2);
        float dOSquared = d/sfVectorMath::dot(orthog, orthog) + 1e-10;

        sf::Vector2f pv = dOSquared*orthog;

        return std::make_pair(false, pv);
    }
    else
    {
        return std::make_pair(true, sf::Vector2f{0.0f, 0.0f});

    }
}

sf::Vector2f Collisions::getCentre(std::vector<sf::Vertex> &obj)
{
    float numVerts = static_cast<float>(obj.size());
    sf::Vector2f average = {0.0f, 0.0f};

    for(sf::Vertex &vert : obj)
    {
        average = average + vert.position;
    }

    return (1.0f/numVerts) * average;
}

std::pair<bool, sf::Vector2f> Collisions::sepAxisTest(std::vector<sf::Vertex> &obj1,
                                          std::vector<sf::Vertex> &obj2)
{
    std::vector<sf::Vector2f > edges = edgesOf(obj1);
    std::vector<sf::Vector2f > edges2 = edgesOf(obj2);
    edges.insert(edges.end(), edges2.begin(), edges2.end());


    std::vector<sf::Vector2f > orthogonals;
    for(sf::Vector2f &edge : edges)
    {
        sf::Vector2f orthog = orthogonal(edge);
        orthogonals.push_back(orthog);
    }

    std::vector<sf::Vector2f > pushVectors;
    for(sf::Vector2f &orthog : orthogonals)
    {
        std::pair<bool, sf::Vector2f> result = isSeparatingAxis(orthog, obj1, obj2);
        if(result.first)
            return std::make_pair(false, sf::Vector2f{0.0f, 0.0f});

        else
            pushVectors.push_back(result.second);
    }

    sf::Vector2f mpv = {1e+15, 1e+15};
    for(sf::Vector2f &pv : pushVectors)
    {
        if(sfVectorMath::dot(pv,pv) < sfVectorMath::dot(mpv, mpv))
            mpv = pv;
    }

    sf::Vector2f d = getCentre(obj1) - getCentre(obj2);

    if(sfVectorMath::dot(d, mpv) > 0)
        mpv = -mpv;

    return std::make_pair(true, mpv);
}
