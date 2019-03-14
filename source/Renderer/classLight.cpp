#include "classLight.h"

LightSource::LightSource(LightProperties properties) :
    lightProperties{properties}, position{properties.position}
{
    renderSubject.notify(*this, Event(EventType::New_LightSrc));
}

LightSource::~LightSource()
{
    renderSubject.notify(*this, Event(EventType::Delete_LightSrc));
}

Subject LightSource::renderSubject;
