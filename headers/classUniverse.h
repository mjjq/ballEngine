#ifndef CLASS_UNIVERSE_H
#define CLASS_UNIVERSE_H
#include "integrators.h"
#include "classBall.h"

class BallUniverse
{
    std::vector<Ball> ballArray;

    int worldSizeX;
    int worldSizeY;
    int numOfBalls = 0;
    int collider1 = 0;
    int collider2 = 0;
    bool enable_forces;
    bool enable_collisions;

    float currentTime = 0;
    float timeToNextColl = 1e+15;
    float dt;
    Integrators intEnum = Integrators::INTEG_EULER;
    bool useRK4 = false;
    bool isPaused = false;

    float sampledt = 5*dt;
    float timeToNextSample = sampledt;
    bool enable_trajectories;

    std::tuple<int,int,float> findShortestCollTime(float t_coll, std::vector<Ball> &ballArray, float dt=1e+10);

    float totalKE = 0;
    float totalGPE = 0;
    float totalEnergy = 0;
    sf::Vector2f totalMomentum = sf::Vector2f{0,0};

    void calcTotalKE(std::vector<Ball> &ballArray);
    void calcTotalMomentum(std::vector<Ball> &ballArray);
    void calcTotalGPE(std::vector<Ball> &ballArray);
    void calcTotalEnergy();

    void physicsLoop();


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
    void toggleRK4();
    void clearSimulation();
    void changeBallColour();

    const int& getWorldSizeX();
    int& getNumOfBalls();
    bool& getCollisionsEnabled();
    bool& getForcesEnabled();
    float& getTotalKE();
    float& getTotalEnergy();
    float& getTimeStep();
    sf::Vector2f& getTotalMomentum();
    bool& getUseRK4();

    void sampleAllPositions();
    void drawSampledPositions(sf::RenderWindow &window);
    void toggleTrajectories();
    sf::Vector2f getBallPosition(int i);
    void pushBall(float force, float relDirection, int i);
};

#endif // CLASS_UNIVERSE_H
