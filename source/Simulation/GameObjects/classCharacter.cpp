#include "classCharacter.h"
#include "sfVectorMath.h"
#include "stringConversion.h"
#include <cmath>
#include <iostream>

Character::Character(Capsule* _collider, CharacterProperties init) :
    collider{_collider}, properties{init}
{
    collider->setMomentInertia(1e+15);
}

void Character::moveSideWays(float input)
{
    if(contactData.size() > 0)
    {
        for(int i=0; i<contactData.size(); ++i)
        {
            sf::Vector2f direction = input*sfVectorMath::orthogonal(contactData[i].normal, 1.0f);

            std::cout << direction << " dir " << i << "\n";
            std::cout << contactData[i].normal << " norm " << i << "\n";
            if(sfVectorMath::dot(direction, contactData[0].normal) <= 0.0f &&
               sfVectorMath::dot(direction, contactData[contactData.size()-1].normal) <= 0.0f &&
               sfVectorMath::dot(collider->getVelocity(), direction) < properties.movementSpeed)
            {
                std::cout << "true" << i << "\n";
                collider->addSolvedVelocity(direction*properties.movementSpeed,
                                            direction*properties.movementSpeed);
            }
        }

    }
    else
    {
        if(input*collider->getVelocity().x < properties.movementSpeed)
            {
                collider->addSolvedVelocity({input*properties.movementSpeed, 0.0f},
                                        {input*properties.movementSpeed, 0.0f});
            }
    }

}

void Character::moveLeft()
{
    moveSideWays(-1.0f);
}

void Character::moveRight()
{
    moveSideWays(1.0f);
}

void Character::jump()
{
    sf::Vector2f direction = collider->contactNormal;
    collider->addSolvedVelocity({0.0f, -properties.jumpPower},
                                {0.0f, -properties.jumpPower});
}
