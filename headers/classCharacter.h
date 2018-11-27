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
};

class Character
{
    CharacterProperties properties;
    void moveSideWays(float input);
public:
    Character(Capsule* _collider, CharacterProperties init);
    std::vector<ContactData > contactData;
    Capsule* collider;

    void moveLeft();
    void moveRight();
    void jump();
};

#endif // CLASS_CHAR_H
