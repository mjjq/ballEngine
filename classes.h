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
    bool isPaused = false;

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
    void setSimStep(float delta);
    void toggleSimPause();
    void toggleCollisions();
    void toggleForces();
    void clearSimulation();
    void changeBallColour();

    std::vector<Ball>* getBallArrayAddress();
    void drawBalls(sf::RenderWindow &windowRef);
};

template <typename T>
class UITextElement : public sf::Text
{
    std::string initialText;
    sf::Font displayFont;
    sf::Vector2f origPosition;
    T *variable;//function which returns the variable to display
    T displayVariable;
    //Template <typename T>;

    bool displayElement = true;
    bool fixedToWindow = false;

public:
    UITextElement(std::string text, sf::Vector2f position, T *var = nullptr);
    void updateElement(sf::RenderWindow &window, sf::View &GUIView);

};


template <typename T>
class UIDebug
{
    sf::Font currentFont;
    std::vector<UITextElement<T>> textArray;

public:
    void addElement(std::string font, std::string str, sf::Vector2f position, T *var = nullptr);
    void renderElements(sf::RenderWindow &window, sf::View &GUIView);
};


class NonSimulationStuff
{
    int windowSizeX;
    int windowSizeY;
    //sf::Text mousePos;
    sf::RenderWindow window{sf::VideoMode(windowSizeX,windowSizeY), "N-body"};
    sf::View worldView = window.getDefaultView();
    sf::View GUIView = window.getDefaultView();
    float currentZoom = 1.0f;
    sf::Time timestep = sf::milliseconds(1000.0/60.0);
    sf::Time minTimeToNextSpawn = sf::milliseconds(500);
    sf::Time timeToNextSpawn = sf::milliseconds(0);
    sf::RectangleShape boundaryRect;
    bool simFitsInWindow;

    sf::Vector2i mousePosOnClick;
    sf::Vector2i mousePosOnPan;
    sf::Vector2i mousePosOnRelease;
    sf::Vector2f recentViewCoords;
    sf::Vector2i wSize;
    int spawnVelFactor;
    float spawnRadius;
    float spawnMass;
    float ballGridSpacing;
    int ballGridHeight;
    int ballGridWidth;

    BallUniverse ballSim;
    UIDebug<float> debugUI;

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
    void mouseEvents(sf::Event &event);
    void keyEvents(sf::Event &event);
    void resizeEvents(sf::Event &event);
    void incTimeStep(sf::Time delta);
    void decTimeStep(sf::Time delta);
    void newLayerEvent(bool keyBool, sf::Event &event);
    float getWindowSizeX();



public:
    NonSimulationStuff(int m_windowSizeX, int m_windowSizeY, int spawnVelFactor,
                        float spawnRadius, float spawnMass, float ballGridSpacing, int ballGridHeight,
                        int ballGridWidth, bool simFitsInWindow, BallUniverse sim);

    void mainLoop();

};



#endif // CLASS_BALL_H
