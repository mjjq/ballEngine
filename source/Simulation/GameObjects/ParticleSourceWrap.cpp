#include "ParticleSourceWrap.h"
#include "ParticleSystem.h"

Subject ParticleSourceWrap::engineNotify;

ParticleSourceWrap::ParticleSourceWrap(unsigned int numParticles,
                       float avgLifetime,
                       VecFunction _pathFunction) : pSource{ParticleSource(numParticles, avgLifetime, _pathFunction)}
{

    engineNotify.notify(*this, Event(EventType::New_PSrc));
}

ParticleSourceWrap::~ParticleSourceWrap()
{
    engineNotify.notify(*this, Event(EventType::Del_PSrc));
}

void ParticleSourceWrap::animate(float dt)
{
    pSource.updateParticles(dt);
}


