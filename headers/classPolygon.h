#ifndef CLASS_Polygon_H
#define CLASS_Polygon_H

#include <deque>
#include "integrators.h"
#include "classPhysicsObject.h"

class Polygon : public PhysicsObject
{
    std::vector<sf::Vertex > vertices;
    sf::Rect<float> boundingOBB;
    sf::Vector2f centrePosition;
    sf::Vector2f size;

    void genBoundingOBB();

public:
    Polygon(std::vector<sf::Vertex > _vertices,
        float _mass,
        sf::Vector2f _initPos,
        sf::Vector2f _initVel,
        float _rotation,
        float _rotRate);
    ~Polygon();

    sf::Rect<float> getGlobalBounds();

    static const ObjectType MY_TYPE = ObjectType::Polygon;
    ObjectType type() const override;

    float getMinSize();
    sf::Vector2f getCoM();
    sf::Rect<float > getBoundingBox();
    std::vector<sf::Vertex > constructVerts();
    float getRotAngle();

    void draw(sf::RenderWindow &_window);


};

#endif // CLASS_UNIVERSE_H

