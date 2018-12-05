#ifndef CLASS_PROJWEAPON_H
#define CLASS_PROJWEAPON_H

#include "classEquipable.h"

enum class ProjectileType
{
    Bullet,
    Bomb,
    _Count
};

class ProjectileWeapon : public Equipable
{
    unsigned int remainingAmmo = 10;
public:
    float muzzleLength;
    float aimingAngle;

    const EquipableType MY_TYPE = EquipableType::PWeapon;
    ProjectileType PROJ_TYPE = ProjectileType::Bullet;
    EquipableType type() const override;

    void primaryFunc();
    void secondaryFunc();
};

#endif // CLASS_WEAPON_H
