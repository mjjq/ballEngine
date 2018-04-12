#ifndef CLASS_BALL_H
#define CLASS_BALL_H

#include <deque>
#include "integrators.h"

class Ball : public sf::CircleShape
{
    sf::Vector2f cStepVelocity;
    sf::Vector2f nStepVelocity = cStepVelocity;
    float dampingFactor = 1;
    float mass;
    float radius;
    bool collidedThisFrame = false;

    float lenJonesForce(float x, float x_0, float r, float m);
    float exptCollForce(float x, float x_0, float r, float m);
    float newtonForce(float x, float x_0, float r, float G, float M);

    std::deque<sf::Vector2f> previousPositions;
    bool samplePreviousPositions = true;

public:
    Ball(float radius, float mass, sf::Vector2f initPos, sf::Vector2f initVel);

    float timeToCollision(Ball &otherBall);
    void checkForBounce(int worldSizeX, int worldSizeY);
    void ballCollision(Ball &otherBall);
    void updateVelocity(Integrators integType, float dt, Ball &otherBall);
    void applyExternalImpulse(sf::Vector2f force, float dt);
    void updatePosition(float dt);
    void sampleNextPosition();
    void sampleCurrentPosition();

    float getMass();
    float getRadius();
    sf::Vector2f getVelocity();
    void setVelocity(sf::Vector2f vel);
    void setToCollided();
    void resetToCollided();
    bool getHasCollided();
    float getKE();
    float getGPE(Ball &otherBall);
    sf::Vector2f getMomentum();
    float getDistance(Ball &otherBall);
    float getSpeed();
    float getRelSpeed(Ball &otherBall);

    bool getSamplePrevPosBool();
    std::deque<sf::Vector2f>& getPreviousPositions();
};

#endif // CLASS_UNIVERSE_H
