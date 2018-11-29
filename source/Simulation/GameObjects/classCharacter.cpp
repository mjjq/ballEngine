#include "classCharacter.h"
#include "sfVectorMath.h"
#include "stringConversion.h"
#include <cmath>
#include <iostream>

float Character::MAX_SLOPE_ANGLE = 50.0f;
float Character::MAX_SLOPE_COSINE = cosf(sfVectorMath::PI * Character::MAX_SLOPE_ANGLE / 180.0f);

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
            if(slopeOkay)
            {
                sf::Vector2f direction = input*sfVectorMath::orthogonal(contactData[i].normal, 1.0f);

                if(sfVectorMath::dot(direction, contactData[0].normal) <= 0.0f &&
                   sfVectorMath::dot(direction, contactData[contactData.size()-1].normal) <= 0.0f &&
                   sfVectorMath::dot(collider->getVelocity(), direction) < properties.movementSpeed)
                {
                    collider->addSolvedVelocity(direction*properties.movementSpeed,
                                                direction*properties.movementSpeed);
                }
            }
        }

    }
    if(contactData.size() == 0 || (!slopeOkay && contactData.size()==2))
    {
        //std::cout << "thing\n";
        //std::cout << slopeOkay << "\n ";
        //std::cout << contactData.size() << " sii\n\n ";
        if(input*collider->getVelocity().x < properties.movementSpeed)
            {
                collider->addSolvedVelocity({0.1f*input*properties.movementSpeed, 0.0f},
                                        {0.1f*input*properties.movementSpeed, 0.0f});
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
    collider->addSolvedVelocity({0.0f, -properties.jumpPower},
                                {0.0f, -properties.jumpPower});
}

void Character::addContactData(ContactData &data)
{
    contactData.push_back(data);
}

void Character::clearContactData()
{
    contactData.clear();
    slopeOkay = true;
}

bool Character::updateState()
{
    //slopeOkay = true;
    //collider->setCoefFriction(properties.coefFriction);

    for(int i=0; i<contactData.size(); ++i)
    {
        float dProduct = sfVectorMath::dot(contactData[i].normal, {0.0f, 1.0f});
        if(dProduct < MAX_SLOPE_COSINE)
        {
            //collider->setCoefFriction(0.0f);
            slopeOkay = false;
            return false;
        }
    }
    return true;
}

bool Character::getSlopeState()
{
    return slopeOkay;
}

Capsule* Character::getColliderAddress()
{
    return collider;
}
