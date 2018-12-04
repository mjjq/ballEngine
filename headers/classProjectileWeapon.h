#ifndef CLASS_PROJWEAPON_H
#define CLASS_PROJWEAPON_H

#include "classEquipable.h"

class ProjectileWeapon : public Equipable
{
    unsigned int remainingAmmo = 10;
public:
    void primaryFunc();
    void secondaryFunc();
};

#endif // CLASS_WEAPON_H
