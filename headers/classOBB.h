#ifndef CLASS_OBB_H
#define CLASS_OBB_H

#include <deque>
#include "integrators.h"
#include "classPhysicsObject.h"

class OBB : public PhysicsObject
{
    sf::Vector2f size;

public:
    OBB(ObjectProperties init);
    ~OBB();

    sf::Rect<float> getGlobalBounds();

    static const ObjectType MY_TYPE = ObjectType::OBB;
    ObjectType type() const override;

    float getMinSize();
    sf::Vector2f getCoM();
    sf::Rect<float > getBoundingBox();
    std::vector<sf::Vertex > constructVerts();
    float getRotAngle();

    void draw(sf::RenderWindow &_window);

    sf::Vertex farthestPointInDir(sf::Vector2f direction);
};

#endif // CLASS_UNIVERSE_H

