#include "ParticleSourceWrap.h"
#include "ParticleSystem.h"

Subject ParticleSourceWrap::engineNotify;

const VecFunction ParticleSourceWrap::defaultPf = [](float t, Particle& p) {
        float constant = 1.0f * (float)(p.seed - RAND_MAX/2)/(float)RAND_MAX;

        float constant2 = 1.0f * (float)(p.seed2 - RAND_MAX/2)/(float)RAND_MAX;

        float f1 = 20.0f*log(1.0f + 1.0f*t);
        float f2 = 20.0f*log(1.0f + 1.0f*t);
        float x = 1.0f*constant * f1;
        float y = 1.0f*constant2 * f2;

        uint8_t value = 255/(1.0f + 0.1f*t);
        uint8_t value2 = 255/(1.0f + 1.0f*t*t*t*t);
        p.color = sf::Color(value, value2, value);
        p.position = sf::Vector2f{x,y} + p.initialPosition;
    };

const VecFunction ParticleSourceWrap::firePf = [](float t, Particle& p) {
        float constant = 1.0f * (float)(p.seed - RAND_MAX/2)/(float)RAND_MAX;

        float constant2 = 1.0f * (float)(p.seed2 - RAND_MAX/2)/(float)RAND_MAX;


        float alpha = 0.3f;
        sf::Vector2f g = 1.0f/alpha * sf::Vector2f{0.0f, -1.0f};
        sf::Vector2f v0 = {5.0f*constant, 5.0f*constant2};

        float exp = 1.0f - pow(2.71828, -alpha * t);
        sf::Vector2f f = t * g +  exp * 1.0f/alpha * (v0 - g) + 5.0f*v0;

        float temp = pow(2.71828, - 0.5f * t / p.lifetime);

        p.color = sf::Color(255 * temp, 255 * temp * temp, 255 * pow(temp, 6), 255 * pow(temp, 5) );
        p.position = f + p.initialPosition;
    };

ParticleSourceWrap::ParticleSourceWrap(unsigned int numParticles,
                       float avgLifetime,
                       VecFunction _pathFunction,
                       std::string const & shaderID) : pSource{ParticleSource(numParticles, avgLifetime, _pathFunction)},
                       vArray{sf::VertexArray(sf::PrimitiveType::Points, numParticles)}
{
    ObjectProperties props;
    props.vArrayCount = numParticles;
    props.material.shaderID = shaderID;
    renderable = new Renderable(props);

    engineNotify.notify(*this, Event(EventType::New_PSrc));
}

ParticleSourceWrap::~ParticleSourceWrap()
{
    delete renderable;
    engineNotify.notify(*this, Event(EventType::Del_PSrc));
}

void ParticleSourceWrap::animate(float dt)
{
    pSource.updateParticles(dt);

    for(unsigned int i=0; i<pSource.getParticleCount(); ++i)
    {
        Particle const & p = pSource.getParticle(i);
        vArray[i].position = p.position;
        vArray[i].color = p.color;
    }

    renderable->primDrawable = &vArray;
}

void ParticleSourceWrap::setPosition(sf::Vector2f const & position)
{
    pSource.setPosition(position);
}


