#ifndef CLASS_GJK_H
#define CLASS_GJK_H

#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "classPolygon.h"

class Simplex
{
    std::vector<sf::Vertex > vertices;
    sf::Vector2f bestDirection;

public:
    bool contains(sf::Vector2f point);
    void add(sf::Vertex vert);
    sf::Vertex getLast();
    sf::Vector2f getNewDir(sf::Vector2f point);
};

class GJK
{
    typedef std::vector<sf::Vertex > ClippedPoints;
    typedef std::vector<sf::Vertex > PolyVerts;

    static sf::Vertex support(PhysicsObject* p1, PhysicsObject* p2,
                              sf::Vector2f &direction);
    static sf::Vertex farthestPointInDir(PolyVerts &p, sf::Vector2f direction);

public:
    static bool isIntersecting(PhysicsObject* p1, PhysicsObject* p2);

    static ClippedPoints getClosestPoints(PolyVerts &p1, PolyVerts &p2);
};


#endif // CLASS_GJK_H
