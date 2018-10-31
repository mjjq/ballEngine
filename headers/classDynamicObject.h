#ifndef CLASS_DYNOBJ_H
#define CLASS_DYNOBJ_H

class DynamicObject
{
public:
    virtual void addSolvedVelocity(sf::Vector2f cStep, sf::Vector2f nStep);
    virtual void updatePosition(float dt);

};

#endif // CLASS_DYNOBJ_H
