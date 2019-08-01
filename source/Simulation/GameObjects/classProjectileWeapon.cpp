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
    muzzleLength = 100.0f;
    //PROJ_TYPE = ProjectileType::Bullet;
    remainingAmmo = 10;

    localOffset = {90.0f, 40.0f};

    anchorPoints.insert({"handle", {-20.0f, -3.0f}});
    //anchorPoints.insert({"grip", {50.0f, -3.0f}});
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
    objProps._mass = 0.1f;
    objProps._ignoreGravity = true;
    objProps._size = {3.0f, 3.0f};
    objProps._position = parentPosition + localMuzzlePos;
    objProps._velocity = parentVelocity + 10.0f*Math::norm(localMuzzlePos);
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


void ProjectileWeapon::setAimAngle(float angle)
{
    aimAngle = angle;

    if(!flipped)
        localMuzzlePos = Math::rotate({muzzleLength, 0.0f}, angle);
    else
        localMuzzlePos = Math::rotate({-muzzleLength, 0.0f}, angle);
}
