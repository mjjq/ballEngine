#include <iostream>
#include "classGameObject.h"
#include "Math.h"

Subject GameObject::engineNotify;

GameObject::GameObject(Renderable* _renderObj,
                       PhysicsObject* _collider,
                       LightSource* _lightSrc,
                       Character* _character,
                       Skeleton2DWrap* _skeleton,
                       Equipable* _equipable) :
                           renderObj{_renderObj},
                           collider{_collider},
                           lightSrc{_lightSrc},
                           character{_character},
                           skeleton{_skeleton},
                           equipable{_equipable}
{
    if(collider != nullptr)
        collider->physSubject.addObserver(this);

    if(skeleton != nullptr)
    {
        skeleton->skelSubject.addObserver(this);

        std::vector<sf::Vector2f > jointPositions = skeleton->getJointPositions();

        ObjectProperties tempProps;

        for(int i=0; i<jointPositions.size(); ++i)
        {
            tempProps._position = jointPositions[i];
            skeletonDebugJoints.push_back(new Renderable(tempProps));
        }
    }

    if(character != nullptr)
    {
        character->charSubject.addObserver(this);
        if(collider != nullptr)
        {
            character->setCollider(collider);
        }
    }

    if(equipable != nullptr)
    {
        equipable->wepSub.addObserver(this);
    }

    engineNotify.notify(*this, Event(EventType::New_GameObject));
}

GameObject::~GameObject()
{
    if(collider != nullptr)
        delete collider;
    if(renderObj != nullptr)
        delete renderObj;
    if(lightSrc != nullptr)
        delete lightSrc;
    if(character != nullptr)
        delete character;
    if(skeleton != nullptr)
    {
        delete skeleton;
        for(int i=0; i<skeletonDebugJoints.size(); ++i)
        {
            delete skeletonDebugJoints[i];
        }
        skeletonDebugJoints.clear();
    }
    if(equipable != nullptr)
        delete equipable;

    engineNotify.notify(*this, Event(EventType::Delete_GameObject));
}


PhysicsObject* GameObject::getColliderAddress()
{
    return collider;
}


void GameObject::addObserver(Observer* obs)
{
    projSub.addObserver(obs);
}


void GameObject::setPosition(sf::Vector2f const & position)
{
    if(collider != nullptr)
    {
        collider->setPosition(position);
    }
    if(renderObj != nullptr)
    {
        renderObj->updatePosition(position);
    }
    if(lightSrc != nullptr)
    {
        lightSrc->position.x = position.x;
        lightSrc->position.y = position.y;
    }
}

void GameObject::setVelocity(sf::Vector2f const & velocity)
{
    if(collider != nullptr)
        collider->setVelocity(velocity);
}

void GameObject::onNotify(Component& entity, Event event, Container* data)
{
    switch(event.type)
    {
        case(EventType::Update_Position):
        {
            sf::Vector2f position = ((DataContainer<sf::Vector2f >& )(*data)).data;
            if(renderObj != nullptr)
            {
                renderObj->updatePosition(position);
                //renderObj->updateOrientation(collider->getRotAngle());
            }
            if(lightSrc != nullptr)
            {
                lightSrc->position.x = position.x;
                lightSrc->position.y = position.y;
            }
            if(skeleton != nullptr)
            {
                skeleton->setRootPosition(position);
            }
            if(character != nullptr)
            {
                character->updateState();
            }
            if(equipable != nullptr)
            {
                equipable->updateParentPos(position);
            }
            break;
        }
        case(EventType::Update_Rotation):
        {
            float rotation = ((DataContainer<float >& )(*data)).data;
            if(renderObj != nullptr)
            {
                renderObj->updateOrientation(rotation);
            }
            if(equipable != nullptr)
            {
                equipable->setAimAngle(rotation);
            }
            break;
        }
        case(EventType::Skel_Animate):
        {
            std::vector<sf::Vector2f > jointPositions = skeleton->getJointPositions();

            for(int i=0; i<jointPositions.size(); ++i)
            {
                skeletonDebugJoints[i]->updatePosition( jointPositions[i] );
            }
            break;
        }
        case(EventType::Projectile_Contact):
        {
            if(character != nullptr)
            {
                character->updateState();
            }
            break;
        }
        case(EventType::Set_Animation):
        {
            if(skeleton != nullptr)
            {
                std::string name = ((DataContainer<std::string>&)*data).data;
                skeleton->setAnimation(name);
            }
            break;
        }
        case(EventType::Character_SetTarget):
        {
            if(skeleton != nullptr)
            {
                sf::Vector2f target = ((DataContainer<sf::Vector2f >&)(*data)).data;
                skeleton->setTarget(target);
                std::cout << target.x << ", " << target.y << "new thing\n";
            }
            break;
        }
        default:
            break;
    }
}
