#ifndef CLASS_Polygon_H
#define CLASS_Polygon_H

#include <deque>
#include "integrators.h"
#include "classPhysicsObject.h"

class Polygon : public PhysicsObject
{
    std::vector<sf::Vertex > vertices = {};
    std::vector<sf::Vector2f > edgeNorms = {};
    std::vector<sf::Vector2f > edgeTotals = {};
    sf::Rect<float> boundingOBB;
    sf::Vector2f centrePosition = {0.0f, 0.0f};
    sf::Vector2f size = {0.0f, 0.0f};

    void genBoundingOBB();

public:
    Polygon(std::vector<sf::Vertex > _vertices,
        ObjectProperties init);
    ~Polygon();

    sf::Rect<float> getGlobalBounds();

    static const ObjectType MY_TYPE = ObjectType::Polygon;
    ObjectType type() const override;

    float getMinSize();
    sf::Vector2f getCoM();
    sf::Rect<float > getBoundingBox();
    std::vector<sf::Vertex > constructVerts();
    std::vector<sf::Vertex > constructLocalVerts();
    std::vector<sf::Vector2f> getLocalEdgeNorms();
    std::vector<sf::Vector2f> getLocalEdgeTotals();
    float getRotAngle();

    void draw(sf::RenderWindow &_window);

    sf::Vertex farthestPointInDir(sf::Vector2f direction);

};

#endif // CLASS_UNIVERSE_H

