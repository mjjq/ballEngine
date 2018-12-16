#include "classProjectileWeapon.h"

#include "sfVectorMath.h"

EquipableType ProjectileWeapon::type() const { return MY_TYPE; }

void ProjectileWeapon::primaryFunc()
{
    wepSub.notify(*this, Event{EventType::Fire_Weapon});
}

void ProjectileWeapon::secondaryFunc()
{

}

sf::Vector2f ProjectileWeapon::getLocalPosition()
{
    return localMuzzlePos;
}


void ProjectileWeapon::changeAimAngle(float angle)
{
    aimAngle = angle;
    localMuzzlePos = sfVectorMath::rotate({muzzleLength, 0.0f}, angle);
}
