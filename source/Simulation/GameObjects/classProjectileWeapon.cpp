#include "classProjectileWeapon.h"

#include "Math.h"
#include "classGameObject.h"

ProjectileWeapon::ProjectileWeapon(EquipableData _data,
                                   WeaponType type) : Equipable(_data)
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
    objProps._velocity = parentVelocity + 50.0f*Math::norm(localMuzzlePos);
    objProps.material = {"",
                         "red.jpg",
                         "normal2.png"};


    VecFunction pF = [](float t, Particle& p) {
        float constant = 1.0f * (float)(p.seed - RAND_MAX/2)/(float)RAND_MAX;

        float constant2 = 1.0f * (float)(p.seed2 - RAND_MAX/2)/(float)RAND_MAX;

        float f1 = 20.0f*log(1.0f + 1.0f*t);
        float f2 = 20.0f*log(1.0f + 1.0f*t);
        float x = 1.0f*constant * f1;
        float y = 1.0f*constant2 * f2;

        uint8_t value = 255/(1.0f + 0.1f*t);
        uint8_t value2 = 255/(1.0f + 0.1f*t*t*t*t);
        p.color = sf::Color(value, value2, value);
        p.position = sf::Vector2f{x,y} + p.initialPosition;
    };




    GameObject* obj = new GameObject(nullptr,//new Renderable(objProps),
                   new Ball(objProps),
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   nullptr,
                   new ParticleSourceWrap(500, 50.0f, pF, "test"));
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
