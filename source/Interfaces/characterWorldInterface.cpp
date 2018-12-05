#include "characterWorldInterface.h"
#include "sfVectorMath.h"

void ICharWorld::checkEquippedWeapon()
{
    for(int i=0; i < (int)characters.size(); ++i)
    {
        Equipable* tempEquip = characters[i].getEquippedItem();
        if(tempEquip != nullptr)
        {
            if(tempEquip->executePrimary)
            {
                equipablePrimary(tempEquip, &characters[i]);
            }
        }
    }
}

void ICharWorld::equipablePrimary(Equipable* item, Character* character)
{
    if(item->type() == EquipableType::PWeapon)
    {
        ProjectileWeapon* weapon = static_cast<ProjectileWeapon*>(item);
        if(weapon->PROJ_TYPE == ProjectileType::Bullet)
        {
            sf::Vector2f spawnPos = character->getColliderAddress()->getPosition();
            spawnPos = spawnPos + sfVectorMath::rotate({weapon->muzzleLength, 0.0f},
                                                       weapon->aimingAngle);
            sf::Vector2f spawnVel = sfVectorMath::rotate({100.0f, 0.0f}, weapon->aimingAngle);
            world.spawnNewObject(false, SpawnObjectType::Ball,
                                  {spawnPos, spawnVel, {2.5f, 2.5f}});
        }
    }
}
