#ifndef CLASS_PROJECTILE_H
#define CLASS_PROJECTILE_H

#include "classBall.h"
#include "Observer.h"
#include "classRenderable.h"

enum class ProjectileType
{
    Bullet,
    Bomb,
    _Count
};

class Projectile : public Entity
{
    PhysicsObject* collider;
    Renderable* renderObj;
    ObjectProperties projProperties;
    ProjectileType projType;

    void initialiseBullet(sf::Vector2f initPos, sf::Vector2f initDir);
    void initialiseBomb(sf::Vector2f initPos, sf::Vector2f initDir);

    Subject projSub;

    float damage = 0.0f;
    std::function<void()> onCollideLambda = [&]{};
public:
    Projectile(ProjectileType type,
               sf::Vector2f initPos,
               sf::Vector2f initDir);
    Projectile(ObjectProperties objProps,
               std::function<void()> onColl = [&]{});
    void onCollide();

    ObjectProperties getProjProps();
    PhysicsObject* getColliderAddress();
    void setColliderAddress(PhysicsObject* object);

    void addObserver(Observer* obs);

    float getDamage();
};

#endif // CLASS_PROJECTILE_H
