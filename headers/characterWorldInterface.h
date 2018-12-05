#ifndef CHARWORLDINT_H
#define CHARWORLDINT_H

#include "classUniverse.h"
#include "classCharacter.h"
#include "classEquipable.h"
#include "classProjectileWeapon.h"

class ICharWorld
{
    BallUniverse& world;
    std::vector<Character >& characters;
public:
    void checkEquippedWeapon();
    void equipablePrimary(Equipable* item, Character* character);
};

#endif // CHARWORLDINT_H
