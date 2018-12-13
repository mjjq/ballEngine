#ifndef CHARWORLDINT_H
#define CHARWORLDINT_H

#include "classUniverse.h"
#include "classCharacterManager.h"
#include "classEquipable.h"
#include "classProjectileWeapon.h"
#include "Observer.h"

class ICharWorld : public Observer
{
    BallUniverse* world;
    CharacterManager* charMan;
public:
    ICharWorld(BallUniverse* _world, CharacterManager* _cMang);
    void checkEquippedWeapon();
    void equipablePrimary(Equipable* item, Character* character);
    void spawnNewCharacter(CharacterProperties init);
    void charContactData();
    virtual void onNotify(Entity& entity, Event event);
};

#endif // CHARWORLDINT_H
