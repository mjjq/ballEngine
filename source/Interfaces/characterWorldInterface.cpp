#include "characterWorldInterface.h"
#include "sfVectorMath.h"

ICharWorld::ICharWorld(BallUniverse* _world,
                       CharacterManager* _cMang,
                       ProjectileManager* _pMang)
{
    world = _world;
    charMan = _cMang;
    projMan = _pMang;
}

/*void ICharWorld::checkEquippedWeapon()
{
    for(int i=0; i < (int)charMan->characters.size(); ++i)
    {
        Equipable* tempEquip = charMan->characters[i].getEquippedItem();
        if(tempEquip != nullptr)
        {
            if(tempEquip->executePrimary)
            {
                equipablePrimary(tempEquip, &charMan->characters[i]);
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
}*/


void ICharWorld::spawnNewCharacter(CharacterProperties init)
{
    ObjectProperties objProps;
    objProps._position = init.position;
    objProps._velocity = init.velocity;
    objProps._mass = init.mass;
    objProps._size = init.size;
    objProps._coefFric = 5.0f;

    std::unique_ptr<Capsule > newCapsule = std::make_unique<Capsule >(objProps);
    Character* newChar = new Character{init, this, newCapsule.get()};

    world->spawnNewObject(std::move(newCapsule));
    charMan->addCharacter(newChar);
}

void ICharWorld::charContactData()
{
    for(int i=0; i<(int)charMan->characters.size(); ++i)
    {
        Character* char1 = charMan->characters[i];
        char1->clearContactData();
        PhysicsObject* collider = char1->getColliderAddress();
        for(auto it = world->arbiters.begin(); it != world->arbiters.end(); ++it)
        {
            if(it->second.obj1 == collider)
            {
                Contact arbContact = it->second.contacts[0];
                ContactData newData{arbContact.position,
                                    arbContact.normal,
                                    arbContact.tangent,
                                    arbContact.rA};
                char1->addContactData(newData);

                if(!char1->updateState())
                    it->second.coefFriction = 0.0f;
            }
            else if(it->second.obj2 == collider)
            {
                Contact arbContact = it->second.contacts[0];
                ContactData newData{arbContact.position,
                                    -arbContact.normal,
                                    -arbContact.tangent,
                                    arbContact.rB};
                char1->addContactData(newData);

                if(!char1->updateState())
                    it->second.coefFriction = 0.0f;
            }
        }
    }
}

void ICharWorld::projContactData()
{
    for(int i=0; i<(int)projMan->projectiles.size(); ++i)
    {
        Projectile* proj = projMan->projectiles[i];
        PhysicsObject* collider = proj->getColliderAddress();
        for(auto it = world->arbiters.begin(); it != world->arbiters.end(); ++it)
        {
            if(it->second.obj1 == collider || it->second.obj2 == collider)
                proj->onCollide();
        }
    }
}

Character* ICharWorld::getProjCharCollision(Projectile& proj)
{
    PhysicsObject* projectileObj = proj.getColliderAddress();

    for(int i=0; i<charMan->characters.size(); ++i)
    {
        Character* charI = charMan->characters[i];
        PhysicsObject* charObj = charI->getColliderAddress();

        ArbiterKey key{projectileObj, charObj};

        auto it = world->arbiters.find(key);
        if(it != world->arbiters.end())
        {
            return charI;
        }
    }
    return nullptr;
}

void ICharWorld::dealDamage(Projectile& proj)
{
    Character* charI = getProjCharCollision(proj);

    if(charI != nullptr)
    {
        float damage = proj.getDamage();

        charI->setHealth(charI->getHealth() - damage);

        if(charI->getHealth() <= 0.0f)
        {
            std::cout << charI << " is dead\n";
        }
    }
}


void ICharWorld::onNotify(Entity& entity, Event event)
{
    switch(event.type)
    {
        case(EventType::Fire_Weapon):
        {
            ProjectileWeapon& currentWep = static_cast<ProjectileWeapon& >(entity);
            sf::Vector2f position = currentWep.getParentPos() + currentWep.getLocalPosition();
            float angle = currentWep.getAimAngle();
            Projectile* proj = new Projectile{currentWep.PROJ_TYPE,
                                                position,
                                                sfVectorMath::rotate({1.0f, 0.0f}, angle)};
            proj->addObserver(this);
            std::unique_ptr<Ball > newBall = std::make_unique<Ball >(proj->getProjProps());
            proj->setColliderAddress(newBall.get());

            projMan->addProjectile(proj);
            world->spawnNewObject(std::move(newBall));

            break;
        }
        case(EventType::Destroy_Projectile) :
        {
            Projectile& proj = static_cast<Projectile& >(entity);
            for(int i=0; i<world->dynamicObjects.size(); ++i)
            {
                if(world->dynamicObjects[i].get() == proj.getColliderAddress())
                {
                    world->removeBall(i);
                    break;
                }
            }
            projMan->removeProjectile(&proj);

            break;
        }
        case(EventType::Character_Contact) :
        {
            charContactData();
            break;
        }
        case(EventType::Projectile_Contact) :
        {
            projContactData();
            break;
        }
        case(EventType::Deal_Damage) :
        {
            dealDamage(static_cast<Projectile& >(entity));
        }
        default :
        {
            break;
        }
    }
}
