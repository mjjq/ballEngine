#ifndef CLASS_CHAR_H
#define CLASS_CHAR_H

#include "classCapsule.h"
#include "classProjectileWeapon.h"
#include "Observer.h"
#include "classInventory.h"

struct ContactData
{
	sf::Vector2f position;
	sf::Vector2f normal;
	sf::Vector2f tangent;
	sf::Vector2f rA;
};

struct CharacterProperties
{
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f size;
    float mass;
    float movementSpeed;
    float jumpPower;
    ObjectType bodyType = ObjectType::Capsule;
    float coefFriction = 4.0f;
    float maxHealth = 10.0f;
    float currentHealth = maxHealth;
};

class Character : public Entity
{
    static float MAX_SLOPE_ANGLE;
    static float MAX_SLOPE_COSINE;
    CharacterProperties properties;

    void moveSideWays(float input);

    PhysicsObject* collider;

    bool slopeOkay = true;
    bool touchingSurface = false;

    Inventory characterItems{};
public:
    Character(CharacterProperties init);
    ~Character();

    static Subject engineNotify;
    Subject charSubject;

    void setCollider(PhysicsObject* _collider);

    void moveLeft();
    void moveRight();
    void jump();
    bool updateState();
    bool getSlopeState();

    void equipablePrimary();
    sf::Vector2f getPosition();
    void changeAimAngle(float angle);

    void setHealth(float health);
    float getHealth();
    CharacterProperties getProperties();

    void switchNextItem();
};

#endif // CLASS_CHAR_H
