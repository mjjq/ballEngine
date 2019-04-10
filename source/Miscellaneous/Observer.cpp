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
    bool uniqueObserver = true;
    for(int i=0; i<(int)observers_.size(); ++i)
        if(observers_[i] == observer)
            uniqueObserver = false;

    if(uniqueObserver)
    {
        observers_.emplace_back(observer);
        numObservers_++;
    }
}

void Subject::removeObserver(Observer* observer)
{
    for(int i=0; i<(int)observers_.size(); ++i)
    {
        if(observers_[i] == observer)
        {
            observers_.erase(observers_.begin() + i);
            numObservers_--;
        }
    }
}
