#include <iostream>
#include "classProjectile.h"
#include "sfVectorMath.h"

Projectile::Projectile(ProjectileType type,
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

Projectile::Projectile(ObjectProperties objProps,
               std::function<void()> onColl)
{
    projType = ProjectileType::_Count;
    projProperties = objProps;
}

void Projectile::onCollide()
{
    onCollideLambda();
}

PhysicsObject* Projectile::getColliderAddress()
{
    return collider;
}

void Projectile::setColliderAddress(PhysicsObject* object)
{
    collider = object;
}

ObjectProperties Projectile::getProjProps()
{
    return projProperties;
}


void Projectile::initialiseBullet(sf::Vector2f initPos, sf::Vector2f initDir)
{
    float bulletSpeed = 1.0f;
    float bulletMass = 5.0f;
    float bulletRadius = 5.5f;
    damage = 2.0f;

    ObjectProperties init;
    init._position = initPos;
    if(sfVectorMath::square(initDir) > 0.0f)
        init._velocity = bulletSpeed * sfVectorMath::norm(initDir);
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

void Projectile::initialiseBomb(sf::Vector2f initPos, sf::Vector2f initDir)
{
    float bulletSpeed = 5.0f;
    float bulletMass = 5.0f;
    float bulletRadius = 5.0f;
    damage = 1.0f;

    ObjectProperties init;
    init._position = initPos;
    if(sfVectorMath::square(initDir) > 0.0f)
        init._velocity = bulletSpeed * sfVectorMath::norm(initDir);
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

void Projectile::addObserver(Observer* obs)
{
    projSub.addObserver(obs);
}

float Projectile::getDamage()
{
    return damage;
}
