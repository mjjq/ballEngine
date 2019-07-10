#include <iostream>
#include "classGameObject.h"
#include "Math.h"

GameObject::GameObject(ProjectileType type,
                       sf::Vector2f initPos,
                       sf::Vector2f initDir)
{
    projType = type;
    switch(type)
    {
        case(ProjectileType::Bullet) :
        {
            initialiseBullet(initPos, initDir);
            break;
        }
        case(ProjectileType::Bomb) :
        {
            initialiseBomb(initPos, initDir);
            break;
        }
        default :
            break;
    }
}

GameObject::GameObject(ObjectProperties objProps,
               std::function<void()> onColl)
{
    projType = ProjectileType::_Count;
    projProperties = objProps;
    renderObj = new Renderable(objProps);
    collider = new Ball(objProps);
    collider->physSubject.addObserver(this);
}

GameObject::GameObject(Renderable* _renderObj,
                       PhysicsObject* _collider,
                       LightSource* _lightSrc,
                       Skeleton2DWrap* _skeleton) :
                           renderObj{_renderObj},
                           collider{_collider},
                           lightSrc{_lightSrc},
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
}

GameObject::~GameObject()
{
    if(collider != nullptr)
        delete collider;
    if(renderObj != nullptr)
        delete renderObj;
    if(lightSrc != nullptr)
        delete lightSrc;
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

void GameObject::onCollide()
{
    onCollideLambda();
}

PhysicsObject* GameObject::getColliderAddress()
{
    return collider;
}

/*void GameObject::setColliderAddress(PhysicsObject* object)
{
    collider = object;
    collider->physSubject.addObserver(this);
}*/

ObjectProperties GameObject::getProjProps()
{
    return projProperties;
}


void GameObject::initialiseBullet(sf::Vector2f initPos, sf::Vector2f initDir)
{
    float bulletSpeed = 1.0f;
    float bulletMass = 5.0f;
    float bulletRadius = 5.5f;
    damage = 2.0f;

    ObjectProperties init;
    init._position = initPos;
    if(Math::square(initDir) > 0.0f)
        init._velocity = bulletSpeed * Math::norm(initDir);
    else
    init._velocity = {bulletSpeed, 0.0f};
    init._mass = bulletMass;
    init._ignoreGravity = true;
    init._size = {bulletRadius, bulletRadius};

    projProperties = init;

    onCollideLambda = [&]{
        projSub.notify(*this, Event{EventType::Deal_Damage});
        projSub.notify(*this, Event{EventType::Destroy_Projectile});
    };
}

void GameObject::initialiseBomb(sf::Vector2f initPos, sf::Vector2f initDir)
{
    float bulletSpeed = 5.0f;
    float bulletMass = 5.0f;
    float bulletRadius = 5.0f;
    damage = 1.0f;

    ObjectProperties init;
    init._position = initPos;
    if(Math::square(initDir) > 0.0f)
        init._velocity = bulletSpeed * Math::norm(initDir);
    else
    init._velocity = {bulletSpeed, 0.0f};
    init._mass = bulletMass;
    init._ignoreGravity = false;
    init._size = {bulletRadius, bulletRadius};

    projProperties = init;

    onCollideLambda = [&]{
        projSub.notify(*this, Event{EventType::Deal_Damage});
        projSub.notify(*this, Event{EventType::Gen_Explosion});
    };
}

void GameObject::addObserver(Observer* obs)
{
    projSub.addObserver(obs);
}

float GameObject::getDamage()
{
    return damage;
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
        }
        default:
            break;
    }
}
