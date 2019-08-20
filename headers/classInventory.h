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

    sf::Vector2f intendedEqPosition;
    sf::Vector2f intendedEqOrientation;
    float intendedEqRotation;

    Equipable* getEquippedItem()
    {
        if(equipableItems.size() <= 0)
            return nullptr;

        return equipableItems[equippedIndex];
    }

public:
    Inventory();
    void initialiseDefault();
    void firePrimary();
    void fireSecondary();
    void updateEquippedPos(sf::Vector2f position);
    void updateEquippedAngle(sf::Vector2f orientation);
    void switchTo(int index);
    void nextItem();
    void addObserver(Observer* obs);
    void setFlippedState(bool isFlipped);
    std::map<std::string, sf::Vector2f > getAnchorPoints();

};

#endif // CLASS_INVENTORY_H
