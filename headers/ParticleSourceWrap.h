#ifndef PSRC_WRAP_H
#define PSRC_WRAP_H

#include "ParticleSystem.h"
#include "Observer.h"

class ParticleSourceWrap : public Component
{
    ParticleSource pSource;

public:
    static Subject engineNotify;
    Subject pSrcSubject;

    ParticleSourceWrap(unsigned int numParticles,
                       float avgLifetime,
                       VecFunction _pathFunction);
    ~ParticleSourceWrap();

    void animate(float dt);
};

#endif // PSRC_WRAP_H
