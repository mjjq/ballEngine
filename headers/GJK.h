#ifndef CLASS_GJK_H
#define CLASS_GJK_H

#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "classPhysicsObject.h"
#include "classConcavePolygonWrap.h"

struct SupportData
{
    sf::Vertex s1;
    sf::Vertex s2;

    sf::Vertex difference;
};

struct Edge
{
    sf::Vector2f v1;
    sf::Vector2f v2;
    sf::Vector2f dir;
    sf::Vector2f max;
};

class Simplex
{
    std::vector<sf::Vertex > vertices;
    std::vector<SupportData > sData;
    sf::Vector2f bestDirection;

public:
    bool contains(sf::Vector2f point);
    void add(sf::Vertex vert);
    void add(SupportData data);
    sf::Vertex getVertex(int i);
    SupportData getSData(int i);
    void setSData(int i, SupportData c);
    sf::Vertex getLast();
    sf::Vector2f getNewDir(sf::Vector2f point);
};


class GJK
{
    typedef std::vector<sf::Vertex > ClippedPoints;
    typedef std::vector<sf::Vertex > PolyVerts;

    static sf::Vertex support(PhysicsObject* p1, PhysicsObject* p2,
                              sf::Vector2f &direction);
    static SupportData supportData(PhysicsObject* p1, PhysicsObject* p2,
                              sf::Vector2f &direction);
    static SupportData supportData(ConcavePolygonWrap const & p1,
                                    ConcavePolygonWrap const & p2,
                                   sf::Vector2f &direction);
    static sf::Vertex farthestPointInDir(PolyVerts &p, sf::Vector2f direction);

    static sf::Vertex closestPToOrigin(sf::Vector2f v1, sf::Vector2f v2);

public:
    static bool isIntersecting(PhysicsObject* p1, PhysicsObject* p2);

    static Edge getClosestPoints(PhysicsObject* p1, PhysicsObject* p2);
    static Edge getClosestPoints(ConcavePolygonWrap const & p1,
                                 ConcavePolygonWrap const & p2);
};


#endif // CLASS_GJK_H
