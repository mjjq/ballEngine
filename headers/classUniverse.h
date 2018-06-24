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
    float minBallSize = 0.001;
    float collAccumulator = 0.0f;

    float currentTime = 0;
    float timeToNextColl = 1e+15;
    float dt;
    float accumulator = 0.0f;
    sf::Clock thresholdTimer;
    Integrators intEnum = Integrators::INTEG_VERLET;
    std::string useRK4 = "";
    bool isPaused = false;

    float sampledt = 5*dt;
    float timeToNextSample = sampledt;
    bool enable_trajectories;
    int currentPlayer = -1;
    std::pair<bool, float> playerInput{false, 0.0f};

    std::tuple<int,int,float> findShortestCollTime(float t_coll, std::vector<Ball> &ballArray, float dt=1e+10);

    float totalKE = 0;
    float totalGPE = 0;
    float totalEnergy = 0;
    sf::Vector2f totalMomentum = sf::Vector2f{0,0};

    void calcTotalKE(std::vector<Ball> &ballArray);
    void calcTotalMomentum(std::vector<Ball> &ballArray);
    void calcTotalGPE(std::vector<Ball> &ballArray);
    void calcTotalEnergy();

    float physicsLoop();


public:
    BallUniverse(int worldSizeX, int worldSizeY, float dt, bool force=true, bool collision=true);

    void universeLoop(sf::Time frameTime, sf::Time frameLimit);

    void updateFirstVelocity(Integrators integType, float dt, Ball &firstBall, Ball &secondBall);
    void updateAllObjects(bool enableForces, float dt);
    float timeToCollision(Ball &firstBall, Ball &secondBall);
    void ballCollision(Ball &firstBall, Ball &secondBall);
    void ballAbsorption(Ball &firstBall, Ball &secondBall, float dt);
    void spawnNewBall(sf::Vector2f position, sf::Vector2f velocity, float radius=1, float mass=1);
    void removeBall(unsigned int index);
    void createBallGrid(int numWide, int numHigh, float spacing, sf::Vector2f centralPosition,
                        sf::Vector2f init_velocity = {0,0}, float ballMass=1, float ballRadius=1);
    void createAltBallGrid(int numWide, int numHigh, float spacing, sf::Vector2f centralPosition,
                        sf::Vector2f init_velocity, float ballMass, float ballRadius);
    void createSPSys(sf::Vector2f centralPosition, sf::Vector2f initVelocity);
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
    Integrators& getUseRK4();

    void sampleAllPositions();
    void drawSampledPositions(sf::RenderWindow &window);
    void toggleTrajectories();
    void togglePlayerTraj();
    sf::Vector2f getBallPosition(unsigned int i);
    void pushBall(float force, float relDirection, int i);
    void pushPlayer(float force, float relDirection);
    void setPlayer(unsigned int playerIndex);
    void splitBalls(int ballIndex, float relDirection, float speed);
    void playerInFunc(float relDirection);

};

#endif // CLASS_UNIVERSE_H
