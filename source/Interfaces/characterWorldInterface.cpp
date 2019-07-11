#include "characterWorldInterface.h"
#include "Math.h"

ICharWorld::ICharWorld(BallUniverse* _world,
                       CharacterManager* _cMang,
                       GameObjectManager* _pMang)
{
    world = _world;
    charMan = _cMang;
    projMan = _pMang;
}



void ICharWorld::onNotify(Entity& entity, Event event)
{
    switch(event.type)
    {
        case(EventType::Fire_Weapon):
        {
            break;
        }
        case(EventType::Destroy_Projectile) :
        {

            break;
        }
        case(EventType::Character_Contact) :
        {
            break;
        }
        case(EventType::Projectile_Contact) :
        {
            break;
        }
        case(EventType::Deal_Damage) :
        {
        }
        case(EventType::Gen_Explosion) :
        {
        }
        default :
        {
            break;
        }
    }
}
