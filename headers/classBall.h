#ifndef CLASS_BALL_H
#define CLASS_BALL_H

class Ball : public sf::CircleShape
{
    sf::Vector2f velocity;
    float dampingFactor = 1;
    float mass;
    float radius;
    bool collidedThisFrame = false;

    float lenJonesForce(float x, float x_0, float r, float m);
    float exptCollForce(float x, float x_0, float r, float m);
    float newtonForce(float x, float x_0, float r, float G, float M);

public:
    Ball(float radius, float mass, sf::Vector2f initPos, sf::Vector2f initVel);

    float timeToCollision(Ball &otherBall);
    void checkForBounce(int worldSizeX, int worldSizeY);
    void ballCollision(Ball &otherBall);
    void updateVelocity(float dt, Ball &otherBall);
    void updatePosition(float dt);

    float getMass();
    float getRadius();
    sf::Vector2f getVelocity();
    void setVelocity(sf::Vector2f vel);
    void setToCollided();
    void resetToCollided();
    bool getHasCollided();
    float getKE();
    float getDistance(Ball &otherBall);
    float getSpeed();
    float getRelSpeed(Ball &otherBall);
};

#endif // CLASS_UNIVERSE_H