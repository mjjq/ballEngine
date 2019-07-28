#ifndef CLASS_INVENTORY_H
#define CLASS_INVENTORY_H

#include <vector>
#include <functional>
#include "classProjectileWeapon.h"

class Inventory
{
    int equippedIndex = 0;
    std::vector<Equipable* > equipableItems;

    int money = 0;


    std::function<sf::Vector2f() > getParentPosition;
    std::function<float() > getParentRotation;
public:
    Inventory(std::function<sf::Vector2f() > _getParentPosition = nullptr,
              std::function<float() > _getParentRotation = nullptr);
    void initialiseDefault();
    void firePrimary();
    void fireSecondary();
    void updateEquippedPos(sf::Vector2f position);
    void updateEquippedAngle(float angle)
    {
        equipableItems[equippedIndex]->setAimAngle(angle);
    }
    void switchTo(int index);
    void nextItem();
    Equipable& getEquippedItem();
    void addObserver(Observer* obs);
};

#endif // CLASS_INVENTORY_H
