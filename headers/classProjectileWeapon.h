#ifndef CLASS_PROJWEAPON_H
#define CLASS_PROJWEAPON_H

#include "classEquipable.h"
#include "classProjectile.h"

class ProjectileWeapon : public Equipable
{
    unsigned int remainingAmmo = 10;
public:
    float muzzleLength = 20.0f;
    sf::Vector2f localMuzzlePos = {0.0f, 0.0f};

    const EquipableType MY_TYPE = EquipableType::PWeapon;
    EquipableType type() const override;

    ProjectileType PROJ_TYPE = ProjectileType::Bullet;

    void primaryFunc();
    void secondaryFunc();

    virtual void changeAimAngle(float angle);
    virtual sf::Vector2f getLocalPosition();
};

#endif // CLASS_WEAPON_H
