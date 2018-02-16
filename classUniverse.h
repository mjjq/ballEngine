#ifndef CLASS_UNIVERSE_H
#define CLASS_UNIVERSE_H
#include "classBall.h"

class BallUniverse
{
    std::vector<Ball> ballArray;

    int worldSizeX;
    int worldSizeY;
    int numOfBalls;
    int collider1 = 0;
    int collider2 = 0;
    bool enable_forces;
    bool enable_collisions;

    float currentTime = 0;
    float timeToNextColl = 1e+15;
    float dt;
    bool isPaused = false;

    std::tuple<int,int,float> findShortestCollTime(float t_coll, std::vector<Ball> &ballArray, float dt=1e+10);
    float getTotalKE(std::vector<Ball> &ballArray);


public:
    BallUniverse(int worldSizeX, int worldSizeY, float dt, bool force=true, bool collision=true);

    void universeLoop();

    void spawnNewBall(sf::Vector2f position, sf::Vector2f velocity, float radius=1, float mass=1);
    void createBallGrid(int numWide, int numHigh, float spacing, sf::Vector2f centralPosition,
                        sf::Vector2f init_velocity = {0,0}, float ballMass=1, float ballRadius=1);
    void drawBalls(sf::RenderWindow &windowRef);

    sf::Vector2i getWorldSize();
    void incSimStep(float delta);
    void decSimStep(float delta);
    void setSimStep(float delta);
    void toggleSimPause();
    void toggleCollisions();
    void toggleForces();
    void clearSimulation();
    void changeBallColour();

    const int* getWorldSizeX();
};

#endif // CLASS_UNIVERSE_H
