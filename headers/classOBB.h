#ifndef CLASS_OBB_H
#define CLASS_OBB_H

#include <deque>
#include "integrators.h"
#include "classPhysicsObject.h"

class OBB : public PhysicsObject
{
    sf::Vector2f size;
    sf::Vector2f centreOfMass;

    float rotAngle;
    float rotRate;

public:
    OBB(sf::Vector2f _size,
        float _mass,
        sf::Vector2f _initPos,
        sf::Vector2f _initVel,
        float _rotation,
        float _rotRate);
    ~OBB();

    sf::Rect<float> getGlobalBounds();

    static const ObjectType MY_TYPE = ObjectType::OBB;
    ObjectType type() const override;

    float getMinSize();
    sf::Vector2f getCoM();
    sf::Rect<float > getBoundingBox();
    float getRotAngle();

    void draw(sf::RenderWindow &_window);


};

#endif // CLASS_UNIVERSE_H

