#include "classCharacter.h"
#include "Math.h"
#include "stringConversion.h"
#include <cmath>
#include <iostream>

Subject Character::engineNotify;

float Character::MAX_SLOPE_ANGLE = 50.0f;
float Character::MAX_SLOPE_COSINE = cosf(Math::PI * Character::MAX_SLOPE_ANGLE / 180.0f);

Character::Character(CharacterProperties init) :
    properties{init}
{
    currentState = new IdleState();
    engineNotify.notify(*this, Event(EventType::New_Character));

    characterItems = Inventory();
    //characterItems.initialiseDefault();
}

Character::~Character()
{
    engineNotify.notify(*this, Event(EventType::Delete_Character));
}

void Character::moveSideWays(float input)
{
    //std::cout << contactData.size() << "\n";
    const std::map<PhysicsObject*, Contact > & contactData = collider->getContactData();
    if(contactData.size() > 0)
    {
        for(auto it = contactData.begin(); it != contactData.end(); ++it)
        {
            float dProduct = Math::dot(it->second.normal, {0.0f, 1.0f});
            if(dProduct >= MAX_SLOPE_COSINE)
            {
                sf::Vector2f direction = input*Math::orthogonal(it->second.normal, 1.0f);

                if(Math::dot(direction, contactData.begin()->second.normal) <= 0.0f &&
                   Math::dot(direction, (std::prev(contactData.end()))->second.normal) <= 0.0f &&
                   Math::dot(collider->getVelocity(), direction) < properties.movementSpeed)
                {
                    std::cout << "movement " << direction.x << " " << direction.y << "\n";
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

void Character::jump()
{
    if(touchingSurface && (slopeOkay || collider->getContactData().size()==2))
        collider->addSolvedVelocity({0.0f, -properties.jumpPower},
                                {0.0f, -properties.jumpPower});
}

void Character::setCollider(PhysicsObject* _collider)
{
    collider = _collider;
    collider->setMomentInertia(1e+15f);
}

void Character::setSkeleton(Skeleton2DWrap* _skeleton)
{
    skeleton = _skeleton;
}

bool Character::updateState()
{
    if(currentState != nullptr)
        currentState->update(*this);

    slopeOkay = true;
    touchingSurface = false;
    const std::map<PhysicsObject*, Contact > & contactData = collider->getContactData();

    int badSlopeCount = 0;

    for(auto it = contactData.begin(); it != contactData.end(); ++it)
    {
        touchingSurface = true;
        float dProduct = Math::dot(it->second.normal, {0.0f, 1.0f});
        if(dProduct < MAX_SLOPE_COSINE)
        {
            ++badSlopeCount;
        }
    }
    if(badSlopeCount == (int)contactData.size())
        slopeOkay = false;

    if(collider->getVelocity().y > 0.0f && !touchingSurface)
    {
        handleInput(Input::Fall);
    }
    if(touchingSurface)
        handleInput(Input::Land);

    if(skeleton != nullptr)
    {
        BoneData torsoData = skeleton->getBoneData("torso");
        updateEquipablePosData(torsoData.position, torsoData.orientation);
        updateEquippedAnchorPoints();
    }

    return true;
}

void Character::handleInput(Input input)
{
    CharacterState* newState = currentState->handleInput(*this, input);

    if(newState != nullptr)
    {
        delete currentState;
        currentState = newState;
        currentState->enterState(*this);
    }

    switch(input)
    {
        case Input::EnableTarget:
            properties.aimingAtTarget = true;
            break;
        case Input::DisableTarget:
            properties.aimingAtTarget = false;
            break;
        case Input::Equip_Primary:
            characterItems.firePrimary();
            break;
        default:
            break;
    }
}

bool Character::getSlopeState()
{
    return slopeOkay;
}


void Character::equipablePrimary()
{
    //characterItems.updateEquippedPos(collider->getPosition());
    //characterItems.firePrimary();
    //equippedItem->updateParentPos(collider->getPosition());
    //equippedItem->primaryFunc();
    //charSubject.notify(*this, Event{EventType::Fire_Bullet});
}

void Character::setHealth(float health)
{
    if(health < 0.0f)
        properties.currentHealth = 0.0f;
    else if(health > properties.maxHealth)
        properties.currentHealth = properties.maxHealth;
    else
        properties.currentHealth = health;
}

float Character::getHealth()
{
    return properties.currentHealth;
}

CharacterProperties Character::getProperties()
{
    return properties;
}

void Character::switchNextItem()
{
    characterItems.nextItem();
}

void Character::setAnimation(std::string const & animationName)
{
    DataContainer<std::string > message{animationName};
    charSubject.notify(*this, Event{EventType::Set_Animation}, &message);
    if(properties.aimingAtTarget == true)
        setTarget(properties.target);
}

void Character::setTarget(sf::Vector2f const & target)
{
    sf::Vector2f realTarget = target;

    properties.target = realTarget;
    if(skeleton != nullptr)
    {
        skeleton->setTarget(realTarget);
        BoneData rootData = skeleton->getBoneData("root");
        sf::Vector2f relPos = target - rootData.position;
        if(Math::dot(rootData.orientation, relPos) < 0.0f)
            flipCharacter(properties.flipped);
    }
    //characterItems.updateEquippedPos(collider->getPosition());

    //sf::Vector2f relVector = target - collider->getPosition();
    //characterItems.updateEquippedAngle(atan2(relVector.x, relVector.y));
}

void Character::updateEquipablePosData(sf::Vector2f const & position,
                                sf::Vector2f const & orientation)
{
    characterItems.updateEquippedPos(position);
    characterItems.updateEquippedAngle(orientation);

}

void Character::flipCharacter(bool & _isflipped)
{
    if(skeleton != nullptr)
    {
        if(_isflipped)
        {
            skeleton->setScale({1.0f, 1.0f});
        }
        else
            skeleton->setScale({-1.0f, 1.0f});
    }

    _isflipped = !_isflipped;

    characterItems.setFlippedState(_isflipped);
}

void Character::updateEquippedAnchorPoints()
{
    if(skeleton != nullptr)
    {
        std::map<std::string, sf::Vector2f > anchorPoints =
                characterItems.getAnchorPoints();

        for(auto it = anchorPoints.begin(); it != anchorPoints.end(); ++it)
        {
            if(it->first == "grip")
                skeleton->setTarget(it->second, "right limb", -1);
            else if(it->first == "handle")
                skeleton->setTarget(it->second, "left limb", -1);
        }
    }
}
