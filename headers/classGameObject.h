#ifndef CLASS_GAMEOBJECT_H
#define CLASS_GAMEOBJECT_H

#include "classBall.h"
#include "Observer.h"
#include "classRenderable.h"
#include "classLight.h"

enum class ProjectileType
{
    Bullet,
    Bomb,
    _Count
};

class GameObject : public Observer
{


    ObjectProperties projProperties;
    ProjectileType projType;

    void initialiseBullet(sf::Vector2f initPos, sf::Vector2f initDir);
    void initialiseBomb(sf::Vector2f initPos, sf::Vector2f initDir);

    Subject projSub;

    float damage = 0.0f;
    std::function<void()> onCollideLambda = [&]{};
public:
    Renderable* renderObj = nullptr;
    PhysicsObject* collider = nullptr;
    LightSource* lightSrc = nullptr;

    GameObject(ProjectileType type,
               sf::Vector2f initPos,
               sf::Vector2f initDir);
    GameObject(ObjectProperties objProps,
               std::function<void()> onColl = [&]{});
    GameObject(Renderable* _renderObj = nullptr,
               PhysicsObject* _collider = nullptr,
               LightSource* _lightSrc = nullptr);
    ~GameObject();
    void onCollide();

    ObjectProperties getProjProps();
    PhysicsObject* getColliderAddress();

    void addObserver(Observer* obs);

    float getDamage();

    void setPosition(sf::Vector2f const & position);
    void setVelocity(sf::Vector2f const & velocity);

    void onNotify(Entity& entity, Event event);
};

#endif // CLASS_PROJECTILE_H
