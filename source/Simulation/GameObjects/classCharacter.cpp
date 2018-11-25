#include "classCharacter.h"
#include <cmath>

Character::Character(Capsule* _collider, CharacterProperties init) :
    collider{_collider}, properties{init}
{
    collider->setMomentInertia(1e+15);
}

void Character::moveLeft()
{
    if(collider->getVelocity().x > -properties.movementSpeed)
        collider->addSolvedVelocity({-properties.movementSpeed, 0.0f},
                                {-properties.movementSpeed, 0.0f});
}

void Character::moveRight()
{
    if(collider->getVelocity().x < properties.movementSpeed)
        collider->addSolvedVelocity({properties.movementSpeed, 0.0f},
                                {properties.movementSpeed, 0.0f});
}

void Character::jump()
{
    collider->addSolvedVelocity({0.0f, -properties.jumpPower},
                                {0.0f, -properties.jumpPower});
}
