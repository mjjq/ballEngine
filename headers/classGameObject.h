#ifndef CLASS_GAMEOBJECT_H
#define CLASS_GAMEOBJECT_H

#include "classBall.h"
#include "Observer.h"
#include "classRenderable.h"
#include "classLight.h"
#include "classSkeleton2DWrap.h"
#include "classCharacter.h"


class GameObject : public Observer
{
    Subject projSub;

    std::vector<Renderable* > skeletonDebugJoints;
public:
    static Subject engineNotify;

    Renderable* renderObj = nullptr;
    PhysicsObject* collider = nullptr;
    LightSource* lightSrc = nullptr;
    Character* character = nullptr;
    Skeleton2DWrap* skeleton = nullptr;
    Equipable* equipable = nullptr;

    GameObject(Renderable* _renderObj = nullptr,
               PhysicsObject* _collider = nullptr,
               LightSource* _lightSrc = nullptr,
               Character* _character = nullptr,
               Skeleton2DWrap* _skeleton = nullptr,
               Equipable* _equipable = nullptr);
    ~GameObject();
    void onCollide();

    PhysicsObject* getColliderAddress();

    void addObserver(Observer* obs);


    void setPosition(sf::Vector2f const & position);
    void setVelocity(sf::Vector2f const & velocity);

    void onNotify(Entity& entity, Event event);
};

#endif // CLASS_PROJECTILE_H
