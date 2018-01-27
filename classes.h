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
    void checkForBounce(int windowSizeX, int windowSizeY);
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

    int windowSizeX;
    int windowSizeY;
    sf::Text mousePos;
    sf::RenderWindow window{sf::VideoMode(windowSizeX,windowSizeY), "N-body"};
    sf::View worldView = window.getDefaultView();
    float currentZoom = 1.0f;

    int numOfBalls;
    int collider1 = 0;
    int collider2 = 0;
    bool enable_forces;
    bool enable_collisions;

    sf::Time minTimeToNextSpawn = sf::milliseconds(500);
    sf::Time timeToNextSpawn = sf::milliseconds(0);
    sf::Time timestep = sf::milliseconds(16.67);
    float currentTime = 0;
    float timeToNextColl = 1e+15;
    float dt;

    int spawnVelFactor;
    float spawnRadius;
    float spawnMass;
    float ballGridSpacing;
    int ballGridHeight;
    int ballGridWidth;

    void debugStuff();
    void spawnNewBall(sf::Vector2f position, sf::Vector2f velocity, float radius=1, float mass=1);
    std::tuple<int,int,float> findShortestCollTime(float t_coll, std::vector<Ball> &ballArray, float dt=1e+10);
    void universeLoop(float dt);
    void universeLoopNoForces(float dt);
    void checkMBPress(sf::Vector2i &initPos, bool mouseType);
    float getTotalKE(std::vector<Ball> &ballArray);

    void createBallGrid(int numWide, int numHigh, float spacing, sf::Vector2f centralPosition,
                        sf::Vector2f init_velocity = {0,0}, float ballMass=1, float ballRadius=1);

    sf::Vector2f velocityFromMouse(sf::Vector2i mousePosOnClick);


public:
    BallUniverse(int m_windowSizeX, int m_windowSizeY, float dt, int spawnVelFactor,
                 float spawnRadius, float spawnMass, float ballGridSpacing, int ballGridHeight,
                                    int ballGridWidth, bool force=true, bool collision=true);
    void universeMainLoop();


};

#endif // CLASS_BALL_H
