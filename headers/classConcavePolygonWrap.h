#ifndef CLASS_CONCPOLY_H
#define CLASS_CONCPOLY_H

#include <SFML/Graphics.hpp>
#include "BoundingSphere.h"
#include "ConcavePolygon.h"
#include <cassert>

class ConcavePolygonWrap : public sf::Shape
{
    cxd::ConcavePolygon polygon;

    typedef std::vector<sf::Vertex > PositionArray;

    std::vector<ConcavePolygonWrap > convexPolys;

    void populateLowestLevelVector();

    ConcavePolygonWrap(std::vector<cxd::Vertex > const & vertices);

    float radius = 0.0f;
    float length = 0.0f;
public:
    ConcavePolygonWrap(PositionArray const & vertices);
    ConcavePolygonWrap();

    PositionArray getVertices(bool const & applyTransform = true) const;

    virtual sf::Vector2f getPoint(std::size_t index) const;
    virtual std::size_t getPointCount() const;

    sf::Vertex farthestPointInDir(sf::Vector2f const & direction) const;

    int getConvexPolyCount() const { return convexPolys.size(); }

    ConcavePolygonWrap & getConvexPoly(int index);
    sf::FloatRect getBounds();

    float getRadius() const { return radius; }
    void setRadius(float _radius);

};

#endif // CLASS_CONCPOLY_H
