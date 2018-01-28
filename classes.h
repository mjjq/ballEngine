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

    std::tuple<int,int,float> findShortestCollTime(float t_coll, std::vector<Ball> &ballArray, float dt=1e+10);
    float getTotalKE(std::vector<Ball> &ballArray);


public:
    BallUniverse(int worldSizeX, int worldSizeY, float dt, bool force=true, bool collision=true);

    void universeLoop();

    void spawnNewBall(sf::Vector2f position, sf::Vector2f velocity, float radius=1, float mass=1);
    void createBallGrid(int numWide, int numHigh, float spacing, sf::Vector2f centralPosition,
                        sf::Vector2f init_velocity = {0,0}, float ballMass=1, float ballRadius=1);
    sf::Vector2i getWorldSize();
    void incSimStep(float delta);
    void decSimStep(float delta);
    std::vector<Ball>* getBallArrayAddress();
    void clearSimulation();
};

class NonSimulationStuff
{
    int windowSizeX;
    int windowSizeY;
    //sf::Text mousePos;
    sf::RenderWindow window{sf::VideoMode(windowSizeX,windowSizeY), "N-body"};
    sf::View worldView = window.getDefaultView();
    float currentZoom = 1.0f;
    sf::Time timestep = sf::milliseconds(16.67);
    sf::Time minTimeToNextSpawn = sf::milliseconds(500);
    sf::Time timeToNextSpawn = sf::milliseconds(0);
    sf::RectangleShape boundaryRect;

    int spawnVelFactor;
    float spawnRadius;
    float spawnMass;
    float ballGridSpacing;
    int ballGridHeight;
    int ballGridWidth;

    BallUniverse ballSim;

    void zoomToMouse(float zoomFactor);
    sf::Vector2f getEffectiveZoom(int worldSizeX, int worldSizeY);
    void checkForViewPan(sf::Vector2i initialPos, sf::Vector2f originalView, int worldSizeX, int worldSizeY, bool keyBool);
    void resetView();
    void adjustViewSize(int sizeX, int sizeY, int worldSizeX, int worldSizeY, float zoom);
    void checkMBPress(sf::Vector2i &initPos, bool mouseType);
    sf::Vector2f velocityFromMouse(sf::Vector2i mousePosOnClick, int spawnVelFactor);
    void drawBalls();
    void changeBoundaryRect(sf::Vector2i worldSize);
    void mouseWheelZoom(bool keyPress, float delta);



public:
    NonSimulationStuff(int m_windowSizeX, int m_windowSizeY, int spawnVelFactor,
                 float spawnRadius, float spawnMass, float ballGridSpacing, int ballGridHeight,
                                    int ballGridWidth,BallUniverse sim);

    void mainLoop();

};

#endif // CLASS_BALL_H
