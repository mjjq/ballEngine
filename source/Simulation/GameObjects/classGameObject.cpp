#include <iostream>
#include "classGameObject.h"
#include "Math.h"

Subject GameObject::engineNotify;

GameObject::GameObject(Renderable* _renderObj,
                       PhysicsObject* _collider,
                       LightSource* _lightSrc,
                       Character* _character,
                       Skeleton2DWrap* _skeleton,
                       Equipable* _equipable,
                       Joint* _joint,
                       ParticleSourceWrap* _pSource) :
                           renderObj{_renderObj},
                           collider{_collider},
                           lightSrc{_lightSrc},
                           character{_character},
                           skeleton{_skeleton},
                           equipable{_equipable},
                           joint{_joint},
                           pSource{_pSource}
{
    if(collider != nullptr)
        collider->physSubject.addObserver(this);

    if(skeleton != nullptr)
    {
        skeleton->skelSubject.addObserver(this);

        std::map<std::string, BoneData > jointData = skeleton->getBoneData();

        ObjectProperties tempProps;
        tempProps._size = {5.0f, 0.0f};

        for(auto it = jointData.begin(); it != jointData.end(); ++it)
        {
            BoneData tempData = it->second;

            tempProps._position = tempData.position;
            tempProps.material.diffuseID = "red.jpg";
            //skeletonDebugJoints.push_back(new Renderable(tempProps));


            tempProps.material.diffuseID = "red.jpg";
            if(it->first == "left hand" || it->first == "right hand") tempProps.material.diffuseID = "blankN.jpg";
            tempProps._position = tempData.position + tempData.length * tempData.orientation;
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
        if(skeleton != nullptr)
            character->setSkeleton(skeleton);
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
        for(int i=0; i<(int)skeletonDebugJoints.size(); ++i)
        {
            delete skeletonDebugJoints[i];
        }
        skeletonDebugJoints.clear();
    }
    if(equipable != nullptr)
        delete equipable;
    if(joint != nullptr)
        delete joint;
    if(pSource != nullptr)
        delete pSource;

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
            if(pSource != nullptr)
                pSource->setPosition(position);
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
            std::map<std::string, BoneData > jointData = skeleton->getBoneData();

            int i=0;
            for(auto it = jointData.begin(); it != jointData.end(); ++it)
            {
                BoneData tempData = it->second;

                //skeletonDebugJoints.at(i)->updatePosition( tempData.position );
                skeletonDebugJoints.at(i)->updatePosition( tempData.position +
                                                       tempData.length * tempData.orientation );

                i+=1;
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
            /*if(skeleton != nullptr)
            {
                sf::Vector2f target = ((DataContainer<sf::Vector2f >&)(*data)).data;
                skeleton->setTarget(target);
            }*/
            break;
        }
        /*case(EventType::Joint_PosUpdate):
        {
            if(joint != nullptr)
            {
                sf::Vector2f jointPos = ((DataContainer<sf::Vector2f >&)(*data)).data;
                joint->
            }
        }*/
        case(EventType::Set_Scale):
        {
            if(renderObj != nullptr)
            {
                sf::Vector2f scale = ((DataContainer<sf::Vector2f >& )(*data)).data;
                renderObj->setScale(scale);
            }
        }
        default:
            break;
    }
}
