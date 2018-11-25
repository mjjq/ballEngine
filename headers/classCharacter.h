#ifndef CLASS_CHAR_H
#define CLASS_CHAR_H

#include "classCapsule.h"

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
    Capsule* collider;
    CharacterProperties properties;
public:
    Character(Capsule* _collider, CharacterProperties init);

    void moveLeft();
    void moveRight();
    void jump();
};

#endif // CLASS_CHAR_H
