#ifndef CLASSES_H
#define CLASSES_H

#include "classUniverse.h"
#include "classUIContainer.h"

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
    sf::Time lastFrameTime = sf::milliseconds(0);
    sf::Time currentFrameTime = sf::milliseconds(1000.0/60.0);
    sf::Clock frameClock;
    sf::RectangleShape boundaryRect;
    bool simFitsInWindow;

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
    int spawnVelFactor;
    float spawnRadius;
    float spawnMass;
    float ballGridSpacing;
    int ballGridHeight;
    int ballGridWidth;

    int playerBallIndex = 0;

    BallUniverse ballSim;

    UIContainer container;

    void increaseMass();


    void zoomToMouse(float zoomFactor);
    sf::Vector2f getEffectiveZoom(int worldSizeX, int worldSizeY);
    void checkForViewPan(sf::Vector2i initialPos, sf::Vector2f originalView, int worldSizeX, int worldSizeY, bool keyBool);
    void focusOnBall(int ballIndex, bool keyBool);

    void resetView();
    void adjustViewSize(int sizeX, int sizeY, int worldSizeX, int worldSizeY);//, float zoom);
    void toggleFullScreen();
    void checkMBPress(sf::Vector2i &initPos, bool mouseType);
    sf::Vector2f velocityFromMouse(sf::Vector2i mousePosOnClick, int spawnVelFactor);
    void changeBoundaryRect(sf::Vector2i worldSize);
    void mouseWheelZoom(bool keyPress, float delta);
    void resetUIClick();
    void clickOnUI();

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



public:
    NonSimulationStuff(int m_windowSizeX, int m_windowSizeY, int spawnVelFactor,
                        float spawnRadius, float spawnMass, float ballGridSpacing, int ballGridHeight,
                        int ballGridWidth, bool simFitsInWindow, BallUniverse sim);

    void mainLoop();

};



#endif // CLASS_BALL_H
