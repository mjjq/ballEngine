#ifndef CLASS_UNIVERSE_H
#define CLASS_UNIVERSE_H
#include "integrators.h"
#include "collisionDetection.h"
#include "classPhysicsObject.h"
#include "classAABB.h"
#include "classPolygon.h"
#include "classCapsule.h"
#include "classCharacter.h"
#include "class2DMatrix.h"
#include "stringConversion.h"
#include "classArbiter.h"
#include "Observer.h"

enum class SpawnObjectType
{
    Ball,
    Polygon,
    Capsule,
    _Count,
};

class BallUniverse : public Entity
{

    int worldSizeX;
    int worldSizeY;
    int numOfBalls = 0;
    int collider1 = 0;
    int collider2 = 0;
    bool enable_forces;
    bool enable_collisions;
    bool universalGravity = false;
    sf::Vector2f uGravityDir{0, 0.2};
    float minBallSize = 0.001;

    float collAccumulator = 0.0f;
    float timeToNextColl = 1e+15;
    Matrix2d colliderArray;
    Matrix2d staticCollArray;
    bool hasCollided = false;
    bool collWithStatic = false;

    float currentTime = 0;
    float dt;
    float accumulator = 0.0f;
    sf::Clock thresholdTimer;
    Integrators intEnum = Integrators::INTEG_VERLET;
    std::string useRK4 = "";
    bool isPaused = false;

    float sampledt = 5*dt;
    float timeToNextSample = sampledt;
    bool enable_trajectories = false;

    void calcCollTimes();
    void findShortestCollTime();

    float totalKE = 0;
    float totalGPE = 0;
    float totalEnergy = 0;
    sf::Vector2f totalMomentum = sf::Vector2f{0,0};

    void calcTotalKE(std::vector<PhysicsObject* > &_dynamicObjects);
    void calcTotalMomentum(std::vector<PhysicsObject* > &_dynamicObjects);
    void calcTotalGPE(std::vector<PhysicsObject* > &_dynamicObjects);
    void calcTotalEnergy();

    bool checkForBounce(PhysicsObject* object);

    float physicsLoop();
    float physicsLoopAbsorb();
    void broadPhase();

    Subject universeSub;
public:
    std::vector<std::unique_ptr<PhysicsObject> > dynamicObjects;
    std::vector<std::unique_ptr<PhysicsObject> > staticObjects;
    std::map<ArbiterKey, Arbiter> arbiters;

    BallUniverse(int worldSizeX, int worldSizeY, float dt, bool force=true, bool collision=true);

    void universeLoop(sf::Time frameTime, sf::Time frameLimit);
    void updateFirstVelocity(Integrators _integType, float _dt, PhysicsObject* obj1, PhysicsObject* obj2);
    void updateAllObjects(bool enableForces, float dt);

    void ballAbsorption(Ball &_firstBall, Ball &_secondBall);
    void spawnNewBall(ObjectProperties init);
    /*void spawnNewRect(sf::Vector2f position,
                      float width,
                      float height,
                      sf::Vector2f velocity,
                      float mass,
                      float rotation);
    void spawnStaticRect(sf::Vector2f position, float width, float height, float rotation);*/
    void spawnStaticBall(ObjectProperties init);
    void spawnNewPoly(ObjectProperties init);
    void spawnStaticPoly(ObjectProperties init);
    void spawnNewObject(bool isStatic, SpawnObjectType type, ObjectProperties init);
    void spawnNewObject(std::unique_ptr<PhysicsObject> obj);

    void removeBall(int index);
    void removeRect(int index);
    void createBallGrid(int numWide, int numHigh, float spacing, sf::Vector2f centralPosition,
                        sf::Vector2f init_velocity = {0,0}, float ballMass=1, float ballRadius=1);
    void createAltBallGrid(int numWide, int numHigh, float spacing, sf::Vector2f centralPosition,
                        sf::Vector2f init_velocity, float ballMass, float ballRadius);
    void createSPSys(sf::Vector2f centralPosition, sf::Vector2f initVelocity);
    void drawBalls(sf::RenderWindow &windowRef);
    void collTimeForBall(unsigned int index);

    sf::Vector2i getWorldSize();
    void incSimStep(float delta);
    void decSimStep(float delta);
    void setSimStep(float delta);
    void toggleSimPause();
    void toggleCollisions();
    void toggleForces();
    void toggleRK4();
    void toggleUGravity();
    void clearSimulation();
    void changeBallColour();

    const int& getWorldSizeX();
    std::string getNumOfBalls();
    std::string getCollisionsEnabled();
    std::string getForcesEnabled();
    std::string getUGravityEnabled();
    std::string getTotalKE();
    std::string getTotalEnergy();
    std::string getTimeStep();
    std::string getTotalMomentum();
    std::string getUseRK4();
    std::string getBallSpeed(unsigned int index);
    int getNumTimesColld(unsigned int index);

    void sampleAllPositions();
    void drawSampledPositions(sf::RenderWindow &window);
    void toggleTrajectories();
    void togglePlayerTraj();
    sf::Vector2f getObjPosition(unsigned int i);
    void splitBalls(int ballIndex, float relDirection, float speed);
    void applyUGravity();

    void createExplosion(sf::Vector2f position,
                         float radiusOfEffect,
                         float strength);

    void newObserver(Observer* obs);
};

#endif // CLASS_UNIVERSE_H
