#include "classProjectileWeapon.h"

#include "Math.h"
#include "classGameObject.h"

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
    //wepSub.notify(*this, Event{EventType::Fire_Weapon});
    ObjectProperties objProps;
    objProps._ignoreGravity = true;
    objProps._size = {10.0f, 10.0f};
    objProps._position = parentPosition + sf::Vector2f{muzzleLength, 0.0f};
    objProps._velocity = {10.0f, 0.0f};
    objProps.material = {"phong",
                                             "red.jpg",
                                             "normal2.png"};
    GameObject* obj = new GameObject(new Renderable(objProps),
                   new Ball(objProps),
                   nullptr,
                   nullptr,
                   nullptr);
    std::cout << "primary\n";
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
