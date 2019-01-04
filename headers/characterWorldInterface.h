#ifndef CHARWORLDINT_H
#define CHARWORLDINT_H

#include "classUniverse.h"
#include "classCharacterManager.h"
#include "classProjectileManager.h"
#include "classEquipable.h"
#include "classProjectileWeapon.h"
#include "Observer.h"

class ICharWorld : public Observer
{
    BallUniverse* world;
    CharacterManager* charMan;
    ProjectileManager* projMan;
public:
    ICharWorld(BallUniverse* _world,
               CharacterManager* _cMang,
               ProjectileManager* _pMang);
    void checkEquippedWeapon();
    void equipablePrimary(Equipable* item, Character* character);
    void spawnNewCharacter(CharacterProperties init);
    void spawnNewProjectile(ProjectileType type,
                            sf::Vector2f position,
                            sf::Vector2f velocity);
    void charContactData();
    void projContactData();
    Character* getProjCharCollision(Projectile& proj);
    void dealDamage(Projectile& proj);
    virtual void onNotify(Entity& entity, Event event);
};

#endif // CHARWORLDINT_H
