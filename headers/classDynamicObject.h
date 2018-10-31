#ifndef CLASS_DYNOBJ_H
#define CLASS_DYNOBJ_H

enum class ObjectType
{
    Ball,
    AABB,
    _Count,
};

class DynamicObject
{
public:
    ~DynamicObject();
    virtual void addSolvedVelocity(sf::Vector2f cStep, sf::Vector2f nStep);
    virtual void updatePosition(float dt);
    //virtual void applyExternalImpulse(sf::Vector2f force, float dt);
    virtual sf::Vector2f getPosition() = 0;
    virtual sf::Vector2f getVelocity() = 0;
    virtual void setVelocity(sf::Vector2f vel) = 0;

    virtual void draw(sf::RenderWindow &_window) = 0;
    virtual ObjectType type() const = 0;
};

#endif // CLASS_DYNOBJ_H