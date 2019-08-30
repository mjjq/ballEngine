#ifndef CLASS_CONCPOLY_H
#define CLASS_CONCPOLY_H

#include "ConcavePolygon.h"
#include <SFML/Graphics.hpp>

class ConcavePolygonWrap : public sf::Shape
{
    ConcavePolygon polygon;

    typedef std::vector<sf::Vertex > PositionArray;

public:
    ConcavePolygonWrap(PositionArray const & vertices);

    PositionArray getVertices(bool const & applyTransform = true);

    virtual sf::Vector2f getPoint(std::size_t index) const;
    virtual std::size_t getPointCount() const;
};

#endif // CLASS_CONCPOLY_H
