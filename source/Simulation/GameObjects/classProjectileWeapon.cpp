#include "classProjectileWeapon.h"

#include "Math.h"

ProjectileWeapon::ProjectileWeapon(WeaponType type) : Equipable()
{
    switch(type)
    {
        case(WeaponType::Rifle) :
        {
                initialiseRifle();
                break;
        }
        case(WeaponType::GrenLauncher) :
        {
                initialiseGrenLaunch();
                break;
        }
        default:
        {
            break;
        }
    }
}

EquipableType ProjectileWeapon::type() const { return MY_TYPE; }

void ProjectileWeapon::initialiseRifle()
{
    muzzleLength = 20.0f;
    //PROJ_TYPE = ProjectileType::Bullet;
    remainingAmmo = 10;
}

void ProjectileWeapon::initialiseGrenLaunch()
{
    muzzleLength = 20.0f;
    //PROJ_TYPE = ProjectileType::Bomb;
    remainingAmmo = 10;
}

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
    localMuzzlePos = Math::rotate({muzzleLength, 0.0f}, angle);
}
