#ifndef CLASSES_H
#define CLASSES_H


#include "classScene.h"
#include "classUniverse.h"
#include "classUIContainer.h"

class GameScene : public Scene
{
    enum SpawnQuantity
    {
        SQ_MASS,
        SQ_RADIUS,
        SQ_DENSITY,
    };

    int windowSizeX = 800;
    int windowSizeY = 800;
    //sf::Text mousePos;
    sf::ContextSettings settings;
    sf::RenderWindow &window;
    sf::View worldView = window.getDefaultView();
    sf::View GUIView = window.getDefaultView();
    float currentZoom = 1.0f;
    sf::Time &timestep;
    sf::Time minTimeToNextSpawn = sf::milliseconds(500);
    sf::Time timeToNextSpawn = sf::milliseconds(0);
    sf::Time &currentFrameTime;
    float &currentFPS;
    sf::Time timeSinceFSample = sf::milliseconds(0);
    std::deque<float> previousFrames;
    sf::Clock frameClock;
    sf::RectangleShape boundaryRect;

    int prevWindowSizeX;
    int prevWindowSizeY;
    sf::Vector2i prevWindowPosition;
    bool isFullScreen = false;

    sf::Vector2i mousePosOnClick;
    sf::Vector2i mousePosOnPan;
    sf::Vector2i mousePosOnRelease;

    std::pair<bool,int> mouseOnUIWhenClicked{false, -1};
    //std::pair<bool,int> mouseOnButtonWhenClicked{false, -1};
    bool clickedWindowToDrag = false;

    sf::Vector2f recentViewCoords;
    sf::Vector2i wSize;
    int spawnVelFactor = 10;
    float spawnRadius = 10.0f;
    float spawnMass = 1.0f;
    float ballGridSpacing = 1.0f;
    int ballGridHeight = 10;
    int ballGridWidth = 10;

    bool simFitsInWindow = true;

    int playerBallIndex = 0;

    BallUniverse ballSim{2000, 2000, 1.0f, true, false};

    UIContainer container;

    void increaseMass();


    void zoomToMouse(float zoomFactor);
    sf::Vector2f getEffectiveZoom(int worldSizeX, int worldSizeY);
    void checkForViewPan(sf::Vector2i initialPos, sf::Vector2f originalView, int worldSizeX, int worldSizeY, bool keyBool);
    void focusOnBall(int ballIndex, bool keyBool);

    void resetView();
    void adjustViewSize(sf::Vector2u newSize);//, float zoom);
    void checkMBPress(sf::Vector2i &initPos, bool mouseType);
    sf::Vector2f velocityFromMouse(sf::Vector2i mousePosOnClick, int spawnVelFactor);
    void changeBoundaryRect(sf::Vector2i worldSize);
    void mouseWheelZoom(bool keyPress, float delta);
    void resetUIClick();
    void clickOnUI();
    void updateFPS(sf::Time interval, float framerate);
    sf::Time sampleNextFrame(sf::Time frameTime);


    void mouseWorldEvents(sf::Event &event);
    void mouseViewEvents(sf::Event &event);
    void mouseUIEvents(sf::Event &event);
    void keyEvents(sf::Event &event);
    void resizeEvents(sf::Event &event);
    void playerKeysDown(int player);

    void incTimeStep(sf::Time delta);
    void decTimeStep(sf::Time delta);
    void newLayerEvent(std::vector<bool> &newLayerKeys, sf::Event &event);
    float getWindowSizeX();

    void setSpawnValues(float value, SpawnQuantity toChange);



public:
    GameScene(sf::RenderWindow &window, sf::Time &targetFTime,
              sf::Time &currentFTime, float &currentFPS);
    void update(sf::RenderWindow &window);
    void load();
    void events(sf::Event &event);
    void redraw(sf::RenderWindow &window);

};



#endif // CLASS_BALL_H
