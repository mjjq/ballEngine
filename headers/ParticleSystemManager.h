#ifndef PSYSMAN_H
#define PSYSMAN_H

#include "Observer.h"
#include "ParticleSourceWrap.h"

class ParticleSystemManager : public Observer
{
    std::vector<ParticleSourceWrap* > pSources;

public:
    ParticleSystemManager();
    ~ParticleSystemManager();
    void updateAll(float deltaTime);

    void addPSource(ParticleSourceWrap* pSrc);
    void removePSource(ParticleSourceWrap* pSrc);
    void removePSource(int index);
    void clearAll();

    void onNotify(Component& entity, Event event, Container* data = nullptr);
};

#endif // PARTICLE_SYSTEM_MANAGER
