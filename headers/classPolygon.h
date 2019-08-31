#ifndef CLASS_Polygon_H
#define CLASS_Polygon_H

#include <deque>
#include "integrators.h"
#include "classPhysicsObject.h"
#include "classConcavePolygonWrap.h"

class Polygon : public PhysicsObject
{
    ConcavePolygonWrap poly;

    void genBoundingOBB();

public:
    Polygon(ConcavePolygonWrap const & _poly);
    Polygon(ObjectProperties init);
    ~Polygon();

    sf::Rect<float > getBoundingBox();

    static const ObjectType MY_TYPE = ObjectType::Polygon;
    ObjectType type() const override;

    virtual float getMinSize();

    sf::Vertex farthestPointInDir(sf::Vector2f direction);

    int getVertexCount() { return poly.getPointCount(); }

    std::vector<sf::Vertex > constructVerts();

    void getConvexBreakdown(std::vector<Polygon > & returnArr);
};

#endif // CLASS_UNIVERSE_H

