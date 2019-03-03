#include "classInventory.h"


Inventory::Inventory()
{
    initialiseDefault();
}

void Inventory::initialiseDefault()
{
    equipableItems.emplace_back(new ProjectileWeapon{WeaponType::Rifle});
    equipableItems.emplace_back(new ProjectileWeapon{WeaponType::GrenLauncher});
}

void Inventory::updateEquippedPos(sf::Vector2f position)
{
    equipableItems[equippedIndex]->updateParentPos(position);
}

void Inventory::firePrimary()
{
    equipableItems[equippedIndex]->primaryFunc();
}

void Inventory::fireSecondary()
{
    equipableItems[equippedIndex]->secondaryFunc();
}

void Inventory::switchTo(int index)
{
    equippedIndex = index % equipableItems.size();
}

void Inventory::nextItem()
{
    switchTo(equippedIndex + 1);
}

Equipable& Inventory::getEquippedItem()
{
    return *equipableItems[equippedIndex];
}

void Inventory::addObserver(Observer* obs)
{
    for(int i=0; i<(int)equipableItems.size(); ++i)
    {
        equipableItems[i]->addObserver(obs);
    }
}
