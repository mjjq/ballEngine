#ifndef PSRC_WRAP_H
#define PSRC_WRAP_H

#include "classRenderable.h"
#include "ParticleSystem.h"
#include "Observer.h"

class ParticleSourceWrap : public Component
{
    static const VecFunction defaultPf;
    static const VecFunction firePf;

    ParticleSource pSource;
    Renderable* renderable = nullptr;
    sf::VertexArray vArray;

public:
    static Subject engineNotify;
    Subject pSrcSubject;

    ParticleSourceWrap(unsigned int numParticles,
                       float avgLifetime,
                       VecFunction _pathFunction = firePf,
                       std::string const & shaderID = "test");
    ~ParticleSourceWrap();

    void animate(float dt);
    void setPosition(sf::Vector2f const & position);
};

#endif // PSRC_WRAP_H
