#include "classConcavePolygonWrap.h"

ConcavePolygonWrap::ConcavePolygonWrap(std::vector<cxd::Vertex > const & vertices)
{
    polygon = cxd::ConcavePolygon(vertices);

    if(vertices.size()==2)
    {
        sf::Vector2f pos1 = {(float)vertices[0].position.x, (float)vertices[0].position.y};
        sf::Vector2f pos2 = {(float)vertices[1].position.x, (float)vertices[1].position.y};
        length = sqrtf(Math::square(pos2 - pos1));
    }

    update();
}

ConcavePolygonWrap::ConcavePolygonWrap() {}

ConcavePolygonWrap::ConcavePolygonWrap(PositionArray const & vertices)
{
    std::vector<cxd::Vertex > vertsToPush;
    for(int i=0; i<vertices.size(); ++i)
    {
        vertsToPush.push_back(cxd::Vertex({vertices[i].position.x, vertices[i].position.y}));
    }

    if(vertices.size()==2)
    {
        length = sqrtf(Math::square(vertices[1].position - vertices[0].position));
    }

    polygon = cxd::ConcavePolygon(vertsToPush);

    polygon.convexDecomp();
    populateLowestLevelVector();


    update();
}

void ConcavePolygonWrap::populateLowestLevelVector()
{
    std::vector<cxd::ConcavePolygon > llPolys;
    polygon.returnLowestLevelPolys(llPolys);

    for(auto & poly : llPolys)
    {
        convexPolys.push_back(ConcavePolygonWrap(poly.getVertices()));
    }

}

std::vector<sf::Vertex > ConcavePolygonWrap::getVertices(bool const & applyTransform) const
{
    PositionArray result;
    int pointCount = getPointCount();
    result.reserve(pointCount);

    if(applyTransform)
        for(int i=0; i<pointCount; ++i)
        {
            cxd::Vec2 point = polygon.getPoint(i);
            result.emplace_back(getTransform() * sf::Vector2f{(float)point.x, (float)point.y});
        }
    else
        for(int i=0; i<pointCount; ++i)
        {
            cxd::Vec2 point = polygon.getPoint(i);
            result.emplace_back(sf::Vector2f{(float)point.x, (float)point.y});
        }

    return result;
}

sf::Vector2f ConcavePolygonWrap::getPoint(std::size_t index) const
{
    cxd::Vec2 point = polygon.getPoint(index);
    return {(float)point.x, (float)point.y};
}

std::size_t ConcavePolygonWrap::getPointCount() const
{
    return polygon.getPointCount();
}

sf::Vertex ConcavePolygonWrap::farthestPointInDir(sf::Vector2f const & direction) const
{
    if(getPointCount() == 1)
        return {getTransform() * getPoint(0)};

    float maxProj = -1e15;
    int bestVert = 0;
    sf::Vector2f rotatedDir = Math::rotate(direction, -getRotation());
    for(int i=0; i<getPointCount(); ++i)
    {
        float projection = Math::dot(getPoint(i), rotatedDir);
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

sf::FloatRect ConcavePolygonWrap::getBounds()
{

    sf::FloatRect rect = getLocalBounds();
    rect.left -= radius;
    rect.top -= (radius+length/2.0f);
    rect.width += 2.0f*radius;
    rect.height += 2.0f*radius + length;
    return getTransform().transformRect(rect);;
}

void ConcavePolygonWrap::setRadius(float _radius)
{
    radius = _radius;
}
