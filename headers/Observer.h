#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>

enum class EventType
{
    Fire_Weapon,
    Destroy_Projectile,
    Character_Contact,
    Projectile_Contact,
    PhysicsObj_OnContact,
    Deal_Damage,
    Gen_Explosion,
    New_Renderable,
    Delete_Renderable,
    New_PhysicsObj,
    Delete_PhysicsObj,
    New_LightSrc,
    Delete_LightSrc,
    Update_Position,
    Update_Rotation,
    New_SkelObj,
    New_Character,
    Delete_Character,
    Delete_SkelObj,
    New_GameObject,
    Delete_GameObject,
    Skel_Animate,
    Set_Animation,
    Character_SetTarget,
    Count
};

class Event
{
public:
    EventType type;
    Event(EventType _type) : type{_type}
    {}
};

class Entity
{

};

template <typename T>
class DataContainer : public Entity
{
public:
    DataContainer() {};
    DataContainer(T _data) : data{_data} {};
    T data;
};

class Observer : public Entity
{
public:
    virtual ~Observer() {}
    virtual void onNotify(Entity& entity, Event event) = 0;
};

class Subject
{
    std::vector<Observer* > observers_;
    int numObservers_ = 0;

public:
    void notify(Entity& entity, Event event);

    void addObserver(Observer* observer);

    void removeObserver(Observer* observer);
};

#endif // OBSERVER_H
