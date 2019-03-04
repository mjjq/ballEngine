#ifndef CLASS_PHYSOBJ_H
#define CLASS_PHYSOBJ_H

#include <deque>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include "Observer.h"

enum class ObjectType
{
    Ball,
    AABB,
    OBB,
    Polygon,
    Capsule,
    _Count,
};

struct ObjectProperties
{
    sf::Vector2f _position;
    sf::Vector2f _velocity = {0.0f, 0.0f};
    sf::Vector2f _size = {1.0f, 1.0f};
    float _mass = 1.0f;
    float _coefFric = 0.0f;
    float _coefRest = 0.0f;
    float _rotation = 0.0f;
    float _rotRate = 0.0f;
    std::vector<sf::Vertex > _vertices = {};
    bool _bullet = false;
    bool _ignoreGravity = false;
    ObjectType type = ObjectType::_Count;
};

class PhysicsObject : public Entity
{
    float coefRestitution = 0.0f;
    float coefFriction = 1.0f;
protected:
    sf::Vector2f position;
    sf::Vector2f nStepPosition = getPosition();
    sf::Vector2f pStepPosition = nStepPosition;

    sf::Vector2f cStepVelocity;
    sf::Vector2f nStepVelocity = cStepVelocity;
    sf::Vector2f cStepModVelocity = {0,0};

    float rotAngle = 0.0f;
    float rotRate = 0.0f;

    float mass = 0.0f;
    float density = 0.0f;
    sf::Vector2f centreOfMass = {0.0f, 0.0f};

    float momentInertia = 0.0f;

    float dampingFactor = 1;
    int numDynColls = 0;

    std::deque<sf::Vector2f> previousPositions;
    bool samplePreviousPositions = false;
    bool isPlayer = false;
    bool bullet = false;
    bool ignoreGravity = false;

public:
    PhysicsObject(ObjectProperties init);
    ~PhysicsObject();

    virtual ObjectType type() const = 0;
    virtual void draw(sf::RenderWindow &_window) = 0;

    virtual float getMinSize() = 0;

    sf::Vector2f getPosition();
    sf::Vector2f getVelocity();
    sf::Vector2f getnStepVelocity();
    float getMass();
    float getDensity();
    float getKE();
    float getSpeed();
    sf::Vector2f getMomentum();
    bool isBullet();
    bool ignoresGravity();

    float getGPE(PhysicsObject* otherObj);
    float getDistance(PhysicsObject* otherObj);
    float getRelSpeed(PhysicsObject* otherObj);

    void setPosition(sf::Vector2f newPosition);
    void updatePosition(float dt);
    void addSolvedVelocity(sf::Vector2f cStep, sf::Vector2f nStep);
    void addRotRate(float _rotRate);
    void setVelocity(sf::Vector2f vel);
    void setMass(float _mass);
    void applyExternalImpulse(sf::Vector2f force, float dt);
    void setCoefFriction(float coef);

    void sampleNextPosition();
    void sampleCurrentPosition();

    bool getSamplePrevPosBool();
    void setSamplePrevPosBool(bool value);
    std::deque<sf::Vector2f>& getPreviousPositions();
    sf::Vector2f getPreviousPosition();

    void printAllVelocities();

    bool getIsPlayer();
    void setIsPlayer(bool value);

    void incTimesCollided();
    int getNumCollTimes();

    sf::Vector2f getCoM();
    float getMomentInertia();
    void setMomentInertia(float i);
    float getRotRate();
    void setRotRate(float _rotRate);

    float getCoefRestitution();
    float getCoefFriction();

    virtual sf::Vertex farthestPointInDir(sf::Vector2f direction) = 0;
    virtual sf::Rect<float > getBoundingBox() = 0;
};

#endif // CLASS_DYNOBJ_H
