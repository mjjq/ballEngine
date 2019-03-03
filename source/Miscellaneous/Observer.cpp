#include "Observer.h"

void Subject::notify(Entity& entity, Event event)
{
    for (int i = 0; i < numObservers_; i++)
    {
      observers_[i]->onNotify(entity, event);
    }
}

void Subject::addObserver(Observer* observer)
{
    observers_.emplace_back(observer);
    numObservers_++;
}

void Subject::removeObserver(Observer* observer)
{

}
