#include <iostream>
#include "classGameObject.h"
#include "Math.h"


GameObject::GameObject(Renderable* _renderObj,
                       PhysicsObject* _collider,
                       LightSource* _lightSrc,
                       Character* _character,
                       Skeleton2DWrap* _skeleton) :
                           renderObj{_renderObj},
                           collider{_collider},
                           lightSrc{_lightSrc},
                           character{_character},
                           skeleton{_skeleton}
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

void GameObject::onNotify(Entity& entity, Event event)
{
    switch(event.type)
    {
        case(EventType::Update_Position):
        {
            if(renderObj != nullptr)
            {
                renderObj->updatePosition(collider->getPosition());
                renderObj->updateOrientation(collider->getRotAngle());
            }
            if(lightSrc != nullptr)
            {
                sf::Vector2f collpos = collider->getPosition();
                lightSrc->position.x = collpos.x;
                lightSrc->position.y = collpos.y;
            }
            if(skeleton != nullptr)
            {
                skeleton->setRootPosition(collider->getPosition());
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
        }
        default:
            break;
    }
}
