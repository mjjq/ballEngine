#include "classInventory.h"
#include "classGameObject.h"

#include "jsonParsing.h"
#include "classPolygon.h"

Inventory::Inventory(std::function<sf::Vector2f() > _getParentPosition,
                     std::function<float() > _getParentRotation) :
                         getParentPosition{_getParentPosition},
                         getParentRotation{_getParentRotation}
{
}

void Inventory::initialiseDefault()
{
    ProjectileWeapon* rifle = new ProjectileWeapon{WeaponType::Rifle};

    ObjectProperties props;
    props._position = {100.0f, 100.0f};
    nlohmann::json j = beParser::loadJsonFromFile("./res/json/gun.json");
    beParser::checkObjectPropertyParams(j, props);

    PhysicsObject* collider = new Polygon(props);
    PositionJoint* joint = new PositionJoint({collider},
                                             getParentPosition,
                                             getParentRotation);


    GameObject* obj1 = new GameObject(new Renderable(props),
                                      collider,
                                      nullptr,
                                      nullptr,
                                      nullptr,
                                      rifle,
                                      joint);


    equipableItems.emplace_back(rifle);
    //GameObject* obj1 = new GameObject(new Renderable())
}

void Inventory::updateEquippedPos(sf::Vector2f position)
{
    equipableItems[equippedIndex]->updateParentPos(position);
}

void Inventory::firePrimary()
{
    equipableItems[equippedIndex]->primaryFunc();
}

void Inventory::fireSecondary()
{
    equipableItems[equippedIndex]->secondaryFunc();
}

void Inventory::switchTo(int index)
{
    equippedIndex = index % equipableItems.size();
}

void Inventory::nextItem()
{
    switchTo(equippedIndex + 1);
}

Equipable& Inventory::getEquippedItem()
{
    return *equipableItems[equippedIndex];
}

void Inventory::addObserver(Observer* obs)
{
    for(int i=0; i<(int)equipableItems.size(); ++i)
    {
        equipableItems[i]->addObserver(obs);
    }
}
