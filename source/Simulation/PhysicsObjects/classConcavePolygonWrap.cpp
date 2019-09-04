#include "classConcavePolygonWrap.h"

ConcavePolygonWrap::ConcavePolygonWrap(std::vector<Vertex > const & vertices)
{
    polygon = ConcavePolygon(vertices);

    std::vector<sf::Vertex > conversion;
    for(int i=0; i<vertices.size(); ++i)
    {
        conversion.push_back(sf::Vertex{{(float)vertices[i].position.x,
                              (float)vertices[i].position.y}});
    }

    bounds = BoundingSphere(conversion);

}

ConcavePolygonWrap::ConcavePolygonWrap() {}

ConcavePolygonWrap::ConcavePolygonWrap(PositionArray const & vertices)
{
    std::vector<Vertex > vertsToPush;
    for(int i=0; i<vertices.size(); ++i)
    {
        vertsToPush.push_back(Vertex({vertices[i].position.x, vertices[i].position.y}));
    }

    polygon = ConcavePolygon(vertsToPush);
    bounds = BoundingSphere(vertices);

    polygon.convexDecomp();
    populateLowestLevelVector();
}

void ConcavePolygonWrap::populateLowestLevelVector()
{
    std::vector<ConcavePolygon > llPolys;
    polygon.returnLowestLevelPolys(llPolys);

    for(auto & poly : llPolys)
    {
        convexPolys.push_back(ConcavePolygonWrap(poly.getVertices()));
    }

}

std::vector<sf::Vertex > ConcavePolygonWrap::getVertices(bool const & applyTransform) const
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
    return {(float)point.x, (float)point.y};
}

std::size_t ConcavePolygonWrap::getPointCount() const
{
    return polygon.getPointCount();
}

sf::Vertex ConcavePolygonWrap::farthestPointInDir(sf::Vector2f const & direction) const
{
    float maxProj = -1e15;
    int bestVert = 0;
    for(int i=0; i<getPointCount(); ++i)
    {
        float projection = Math::dot(getTransform() * getPoint(i), direction);
        if(projection > maxProj)
        {
            maxProj = projection;
            bestVert = i;
        }
    }

    return {getTransform() * getPoint(bestVert)};
}

ConcavePolygonWrap & ConcavePolygonWrap::getConvexPoly(int index)
{
    assert(index < convexPolys.size() && index >= 0);

    convexPolys[index].setPosition(getPosition());
    convexPolys[index].setRotation(getRotation());
    convexPolys[index].setRadius(radius);
    return convexPolys[index];
}

BoundingSphere ConcavePolygonWrap::getBounds()
{
    BoundingSphere transformedBounds = bounds;
    transformedBounds.position = getPosition() + transformedBounds.position;
    return transformedBounds;
}

void ConcavePolygonWrap::setRadius(float _radius)
{
    radius = _radius;
    bounds.radius = radius;
}
