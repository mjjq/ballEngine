#include "classSkeleton2DManager.h"

Skeleton2DManager::Skeleton2DManager()
{
    Skeleton2DWrap::engineNotify.addObserver(this);
}

Skeleton2DManager::~Skeleton2DManager()
{
     Skeleton2DWrap::engineNotify.removeObserver(this);
}

void Skeleton2DManager::updateAll(float deltaTime)
{
    for(int i=0; i<skeletons.size(); ++i)
    {
        skeletons[i]->animate(deltaTime);
    }
}


void Skeleton2DManager::addSkeleton(Skeleton2DWrap* skel)
{
    skeletons.push_back(skel);
}

void Skeleton2DManager::removeSkeleton(Skeleton2DWrap* skel)
{
    int i=0;
    bool skelFound = false;

    while(i<(int)skeletons.size() && !skelFound)
    {
        if(skeletons[i] == skel)
        {
            removeSkeleton(i);
            skelFound = true;
        }
        ++i;
    }
}

void Skeleton2DManager::removeSkeleton(int index)
{
    if(std::abs(index) < (int)skeletons.size())
    {
        if(index >=0)
        {
            skeletons.erase(skeletons.begin() + index);
        }
        else if(index < 0)
        {
            skeletons.erase(skeletons.begin() + skeletons.size() + index);
        }
    }
    else if(index == -1 && skeletons.size() == 1)
    {
        delete skeletons[0];
        skeletons.clear();
    }
}

void Skeleton2DManager::clearAll()
{
    for(int i=0; i<(int)skeletons.size(); ++i)
    {
        delete skeletons[i];
    }
    skeletons.clear();
}

void Skeleton2DManager::onNotify(Entity& entity, Event event)
{
    switch(event.type)
    {
        case(EventType::New_SkelObj):
        {
            addSkeleton((Skeleton2DWrap*)(&entity));
            break;
        }
        case(EventType::Delete_SkelObj):
        {
            removeSkeleton((Skeleton2DWrap*)(&entity));
            break;
        }
    }
}
