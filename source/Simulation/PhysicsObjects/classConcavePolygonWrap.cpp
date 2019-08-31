#include "classConcavePolygonWrap.h"

ConcavePolygonWrap::ConcavePolygonWrap(PositionArray const & vertices)
{
    std::vector<Vertex > vertsToPush;
    for(int i=0; i<vertices.size(); ++i)
    {
        vertsToPush.push_back(Vertex({vertices[i].position.x, vertices[i].position.y}));
    }

    polygon = ConcavePolygon(vertsToPush);

    polygon.convexDecomp();
}

std::vector<sf::Vertex > ConcavePolygonWrap::getVertices(bool const & applyTransform)
{
    PositionArray result;

    for(int i=0; i<getPointCount(); ++i)
    {
        if(applyTransform)
            result.push_back({getTransform() * getPoint(i)});
        else
            result.push_back({getPoint(i)});
    }

    return result;
}

sf::Vector2f ConcavePolygonWrap::getPoint(std::size_t index) const
{
    Vec2 point = polygon.getPoint(index);
    return {point.x, point.y};
}

std::size_t ConcavePolygonWrap::getPointCount() const
{
    return polygon.getPointCount();
}
