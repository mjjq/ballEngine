#include "classEquipable.h"
#include "Math.h"
#include <iostream>

void Equipable::setAimAngle(float angle)
{
    aimAngle = angle;
}

float Equipable::getAimAngle()
{
    return aimAngle;
}

sf::Vector2f Equipable::getLocalPosition()
{
    return {0.0f, 0.0f};
}

void Equipable::updateParentPos(sf::Vector2f pos)
{
    parentPosition = pos;
}

void Equipable::updateParentVelocity(sf::Vector2f const & vel)
{
    parentVelocity = vel;
}

sf::Vector2f Equipable::getParentPos()
{
    return parentPosition;
}

void Equipable::addObserver(Observer* obs)
{
    wepSub.addObserver(obs);
}

void Equipable::setFlippedState(bool _flipped)
{
    flipped = _flipped;

    if(_flipped == false)
    {
        DataContainer<sf::Vector2f > scaleData{{1.0f, 1.0f}};
        wepSub.notify(*this, Event{EventType::Set_Scale}, &scaleData);
    }
    else
    {
        DataContainer<sf::Vector2f > scaleData{{-1.0f, 1.0f}};
        wepSub.notify(*this, Event{EventType::Set_Scale}, &scaleData);
    }
}

std::map<std::string, sf::Vector2f > Equipable::getAnchorPoints()
{
    std::map<std::string, sf::Vector2f > transformedPoints(data.anchorPoints);

    for(auto it = transformedPoints.begin(); it != transformedPoints.end(); ++it)
    {
        it->second = Math::rotate(it->second, aimAngle);
        it->second += parentPosition;
    }

    return transformedPoints;
}
