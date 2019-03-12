#include "classLight.h"

LightSource::LightSource(sf::Vector2f _position,
                sf::Vector3f _color) :
                    color{_color}
{
    position = sf::Vector3f(_position.x, _position.y, 1.0f);
    renderSubject.notify(*this, Event(EventType::New_LightSrc));
}

LightSource::~LightSource()
{
    renderSubject.notify(*this, Event(EventType::Delete_LightSrc));
}

Subject LightSource::renderSubject;
