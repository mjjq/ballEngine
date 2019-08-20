#ifndef CHARWORLDINT_H
#define CHARWORLDINT_H

#include "classUniverse.h"
#include "classCharacterManager.h"
#include "classGameObjectManager.h"
#include "classEquipable.h"
#include "classProjectileWeapon.h"
#include "Observer.h"

class ICharWorld : public Observer
{
    BallUniverse* world;
    CharacterManager* charMan;
    GameObjectManager* projMan;
public:
    ICharWorld(BallUniverse* _world,
               CharacterManager* _cMang,
               GameObjectManager* _pMang);
    //void checkEquippedWeapon();
    //void equipablePrimary(Equipable* item, Character* character);
    //void spawnNewCharacter(CharacterProperties init);
    //void spawnNewProjectile(ProjectileType type,
    //                        sf::Vector2f position,
    //                        sf::Vector2f velocity);
    //void spawnNewProjectile(ObjectProperties objProps);
    //void charContactData();
    //void projContactData();
    //Character* getProjCharCollision(GameObject& proj);
    //void dealDamage(GameObject& proj);
    virtual void onNotify(Component& entity, Event event, Container* data = nullptr);
};

#endif // CHARWORLDINT_H
