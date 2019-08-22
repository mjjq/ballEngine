#include "ParticleSystemManager.h"

ParticleSystemManager::ParticleSystemManager()
{
    ParticleSourceWrap::engineNotify.addObserver(this);
}

ParticleSystemManager::~ParticleSystemManager()
{
     ParticleSourceWrap::engineNotify.removeObserver(this);
}

void ParticleSystemManager::updateAll(float deltaTime)
{
    for(int i=0; i<pSources.size(); ++i)
    {
        pSources[i]->animate(deltaTime);
    }
}


void ParticleSystemManager::addPSource(ParticleSourceWrap* pSrc)
{
    pSources.push_back(pSrc);
}

void ParticleSystemManager::removePSource(ParticleSourceWrap* pSrc)
{
    int i=0;
    bool pSrcFound = false;

    while(i<(int)pSources.size() && !pSrcFound)
    {
        if(pSources[i] == pSrc)
        {
            removePSource(i);
            pSrcFound = true;
        }
        ++i;
    }
}

void ParticleSystemManager::removePSource(int index)
{
    if(std::abs(index) < (int)pSources.size())
    {
        if(index >=0)
        {
            pSources.erase(pSources.begin() + index);
        }
        else if(index < 0)
        {
            pSources.erase(pSources.begin() + pSources.size() + index);
        }
    }
    else if(index == -1 && pSources.size() == 1)
    {
        delete pSources[0];
        pSources.clear();
    }
}

void ParticleSystemManager::clearAll()
{
    for(int i=0; i<(int)pSources.size(); ++i)
    {
        delete pSources[i];
    }
    pSources.clear();
}

void ParticleSystemManager::onNotify(Component& entity, Event event, Container* data)
{
    switch(event.type)
    {
        case(EventType::New_PSrc):
        {
            addPSource((ParticleSourceWrap*)(&entity));
            break;
        }
        case(EventType::Del_PSrc):
        {
            removePSource((ParticleSourceWrap*)(&entity));
            break;
        }
    }
}
