#ifndef CLASS_PHYSOBJ_H
#define CLASS_PHYSOBJ_H

#include <deque>

enum class ObjectType
{
    Ball,
    AABB,
    OBB,
    Polygon,
    _Count,
};

class PhysicsObject
{
protected:
    sf::Vector2f position;
    sf::Vector2f nStepPosition = getPosition();
    sf::Vector2f pStepPosition = nStepPosition;

    sf::Vector2f cStepVelocity;
    sf::Vector2f nStepVelocity = cStepVelocity;
    sf::Vector2f cStepModVelocity = {0,0};

    float rotAngle = 0.0f;
    float rotRate = 0.0f;

    float mass;
    float density;
    sf::Vector2f centreOfMass;

    float momentInertia;

    float dampingFactor = 1;
    int numDynColls = 0;

    std::deque<sf::Vector2f> previousPositions;
    bool samplePreviousPositions = false;
    bool isPlayer = false;

public:
    PhysicsObject(sf::Vector2f _position,
                  sf::Vector2f _velocity,
                  float _mass);
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
    float getRotRate();
    void setRotRate(float _rotRate);
};

#endif // CLASS_DYNOBJ_H
