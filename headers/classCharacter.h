#ifndef CLASS_CHAR_H
#define CLASS_CHAR_H

#include "classCapsule.h"
#include "classProjectileWeapon.h"
#include "Observer.h"

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
    float coefFriction = 4.0f;
};


class Character : public Entity
{
    Subject charSubject;
    static float MAX_SLOPE_ANGLE;
    static float MAX_SLOPE_COSINE;
    Capsule* collider;
    CharacterProperties properties;
    void moveSideWays(float input);
    std::vector<ContactData > contactData;

    bool slopeOkay = true;
    bool touchingSurface = false;

    Equipable* equippedItem;

public:
    Character(CharacterProperties init, Observer* obs = nullptr, Capsule* rigidBind = nullptr);

    void moveLeft();
    void moveRight();
    void jump();
    void addContactData(ContactData &data);
    void clearContactData();
    bool updateState();
    bool getSlopeState();
    Capsule* getColliderAddress();
    Equipable* getEquippedItem();

    void equipablePrimary();
};

#endif // CLASS_CHAR_H
