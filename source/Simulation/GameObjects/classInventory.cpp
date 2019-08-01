#include "classInventory.h"
#include "classGameObject.h"

#include "jsonParsing.h"
#include "classPolygon.h"

Inventory::Inventory()
{
}

void Inventory::initialiseDefault()
{
    ProjectileWeapon* rifle = new ProjectileWeapon{WeaponType::Rifle};

    ObjectProperties props;
    props._position = {00.0f, 20.0f};
    nlohmann::json j = beParser::loadJsonFromFile("./res/json/gun.json");
    beParser::checkObjectPropertyParams(j, props);

    PhysicsObject* collider = new Polygon(props);
    PositionJoint* joint = new PositionJoint({collider},
                                             [&]{return intendedEqPosition;},
                                             [&]{return intendedEqRotation;});


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
    sf::Vector2f equippedOffset = {0.0f, 0.0f};

    if(getEquippedItem() != nullptr)
        equippedOffset = equipableItems[equippedIndex]->getLocalOffset();

    sf::Vector2f orthogonal = Math::orthogonal(intendedEqOrientation, 1.0f);

    sf::Vector2f newPosition = position + equippedOffset.y * intendedEqOrientation
                                + equippedOffset.x * orthogonal;

    intendedEqPosition = newPosition;
}

void Inventory::updateEquippedAngle(sf::Vector2f orientation)
{
    intendedEqOrientation = orientation;
    intendedEqRotation = 180.0f/Math::PI * atan2f(orientation.y, orientation.x);

    if(getEquippedItem() != nullptr)
        if(getEquippedItem()->getFlippedState() == true)
            intendedEqRotation += 180.0f;
}

void Inventory::firePrimary()
{
    if(getEquippedItem() != nullptr)
        getEquippedItem()->primaryFunc();
}

void Inventory::fireSecondary()
{
    if(getEquippedItem() != nullptr)
        getEquippedItem()->secondaryFunc();
}

void Inventory::switchTo(int index)
{
    equippedIndex = index % equipableItems.size();
}

void Inventory::nextItem()
{
    switchTo(equippedIndex + 1);
}

void Inventory::setFlippedState(bool isFlipped)
{
    if(getEquippedItem() != nullptr)
        getEquippedItem()->setFlippedState(isFlipped);
}

std::map<std::string, sf::Vector2f > Inventory::getAnchorPoints()
{
    if(getEquippedItem() == nullptr)
        return {};

    return getEquippedItem()->getAnchorPoints();
}


void Inventory::addObserver(Observer* obs)
{
    for(int i=0; i<(int)equipableItems.size(); ++i)
    {
        equipableItems[i]->addObserver(obs);
    }
}
