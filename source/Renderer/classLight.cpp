#include "classLight.h"

LightSource::LightSource(sf::Vector3f _position,
                sf::Vector3f _color) : position{_position},
                    color{_color}
{
    linear = 0.005f;
    renderSubject.notify(*this, Event(EventType::New_LightSrc));
}

LightSource::~LightSource()
{
    renderSubject.notify(*this, Event(EventType::Delete_LightSrc));
}

Subject LightSource::renderSubject;
