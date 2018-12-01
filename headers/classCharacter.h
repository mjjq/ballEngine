#ifndef CLASS_CHAR_H
#define CLASS_CHAR_H

#include "classCapsule.h"

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


class Character
{
    Capsule* collider;
    CharacterProperties properties;
    void moveSideWays(float input);
    std::vector<ContactData > contactData;

    bool slopeOkay = true;

    static float MAX_SLOPE_ANGLE;
    static float MAX_SLOPE_COSINE;
public:
    Character(Capsule* _collider, CharacterProperties init);

    void moveLeft();
    void moveRight();
    void jump();
    void addContactData(ContactData &data);
    void clearContactData();
    bool updateState();
    bool getSlopeState();
    Capsule* getColliderAddress();
};

#endif // CLASS_CHAR_H
