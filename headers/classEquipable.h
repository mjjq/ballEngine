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

struct EquipableData
{
    sf::Vector2f localOffset;
    std::map<std::string, sf::Vector2f > anchorPoints;
};

class Equipable : public Component
{
protected:
    float aimAngle = 0.0f;
    sf::Vector2f parentPosition;
    sf::Vector2f parentVelocity;
    bool flipped = false;

    EquipableData data;
public:
    Equipable(EquipableData _data) : data{_data} {}
    virtual ~Equipable() {}
    Subject wepSub;
    bool executePrimary = false;
    bool executeSecondary = false;

    virtual EquipableType type() const = 0;
    virtual void primaryFunc() = 0;
    virtual void secondaryFunc() = 0;
    virtual sf::Vector2f getLocalPosition();
    virtual void setAimAngle(float angle);
    float getAimAngle();
    void addObserver(Observer* obs);
    void updateParentPos(sf::Vector2f pos);
    void updateParentVelocity(sf::Vector2f const & vel);
    sf::Vector2f getParentPos();
    void setFlippedState(bool _flipped);
    bool getFlippedState() {return flipped;}
    sf::Vector2f getLocalOffset()
    {
        sf::Vector2f newOffset = data.localOffset;
        if(!flipped) newOffset.x *= -1.0f;
        return newOffset;
    }
    std::map<std::string, sf::Vector2f > getAnchorPoints();
};

#endif // CLASS_EQUIPABLE_H
