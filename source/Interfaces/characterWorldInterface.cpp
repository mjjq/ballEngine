#include "characterWorldInterface.h"
#include "sfVectorMath.h"

ICharWorld::ICharWorld(BallUniverse* _world, CharacterManager* _cMang)
{
    world = _world;
    charMan = _cMang;
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


void ICharWorld::onNotify(Entity& entity, Event event)
{
    switch(event.type)
    {
        case(EventType::Fire_Bullet):
        {
            Character& currentChar = static_cast<Character& >(entity);
            sf::Vector2f position = currentChar.getColliderAddress()->getPosition();
            position = position + sf::Vector2f{20.0f, -10.0f};
            ObjectProperties temp;
            temp._position = position;
            temp._velocity = {20.0f, 0.0f};
            temp._size = {3.0f, 3.0f};
            temp._ignoreGravity = true;
            world->spawnNewObject(false, SpawnObjectType::Ball, temp);
            std::cout << "bullet fired\n";
            //world->spawnObject(new GameObject);
            break;
        }
        case(EventType::Character_Contact) :
        {
            charContactData();
        }
        default :
        {
            break;
        }
    }
}
