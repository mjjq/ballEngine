#ifndef CLASS_EQUIPABLE_H
#define CLASS_EQUIPABLE_H

#include "SFML/Graphics.hpp"
#include "Observer.h"

enum class EquipableType
{
    PWeapon,
    MWeapon,
    _Count
};

class Equipable : public Entity
{
protected:
    Subject wepSub;
    float aimAngle = 0.0f;
    sf::Vector2f parentPosition;
public:
    bool executePrimary = false;
    bool executeSecondary = false;

    virtual EquipableType type() const = 0;
    virtual void primaryFunc() = 0;
    virtual void secondaryFunc() = 0;
    virtual sf::Vector2f getLocalPosition();
    virtual void changeAimAngle(float angle);
    float getAimAngle();
    void addObserver(Observer* obs);
    void updateParentPos(sf::Vector2f pos);
    sf::Vector2f getParentPos();
};

#endif // CLASS_EQUIPABLE_H
