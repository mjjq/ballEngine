#ifndef CLASS_INVENTORY_H
#define CLASS_INVENTORY_H

#include <vector>
#include "classProjectileWeapon.h"

class Inventory
{
    int equippedIndex = 0;
    std::vector<Equipable* > equipableItems;

    int money = 0;

    void initialiseDefault();
public:
    Inventory();
    void firePrimary();
    void fireSecondary();
    void updateEquippedPos(sf::Vector2f position);
    void switchTo(int index);
    void nextItem();
    Equipable& getEquippedItem();
    void addObserver(Observer* obs);
};

#endif // CLASS_INVENTORY_H
