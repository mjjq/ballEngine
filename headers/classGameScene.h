#ifndef CLASS_GAMESCENE_H
#define CLASS_GAMESCENE_H


#include "classScene.h"
#include "classUniverse.h"
#include "classUIContainer.h"

class GameScene : public Scene
{
protected:
    enum SpawnQuantity
    {
        SQ_MASS,
        SQ_RADIUS,
        SQ_DENSITY,
    };

    float currentZoom = 1.0f;

    sf::Vector2i mousePosOnClick;
    sf::Vector2i mousePosOnPan;
    sf::Vector2i mousePosOnRelease;

    void resetCamera();
    void adjustViewSize(sf::Vector2u newSize);//, float zoom);

    void checkMBPress(sf::Vector2i &initPos, bool mouseType);

    void mouseViewEvents(sf::Event &event);

    sf::Time &targetFrameTime;
    sf::Time &currentFrameTime;
    float &currentFPS;

    sf::Vector2f recentViewCoords;
    sf::Vector2i wSize;
    int spawnVelFactor = 10;
    float spawnRadius = 10.0f;
    float spawnMass = 1.0f;


    BallUniverse ballSim{2000, 2000, 1.0f, false, false};
    sf::RectangleShape boundaryRect;

    bool canZoom = false;

    //simulation parameters
    sf::Time minTimeToNextSpawn = sf::milliseconds(500);
    sf::Time timeToNextSpawn = sf::milliseconds(0);

    bool simFitsInWindow = true;
    int playerBallIndex = 0;
    bool isLoaded = false;


    //simulation functions
    void zoomToMouse(float zoomFactor);
    sf::Vector2f getEffectiveZoom(int worldSizeX, int worldSizeY);
    void checkForViewPan(sf::Vector2i initialPos);
    void focusOnBall(int ballIndex);
    void spawnFromJson(sf::Vector2f position, sf::Vector2f velocity);



    sf::Vector2f velocityFromMouse(sf::Vector2i mousePosOnClick, int spawnVelFactor);
    void changeBoundaryRect(sf::Vector2i worldSize);
    void mouseWheelZoom(bool keyPress, float delta);

    virtual void mouseWorldEvents(sf::Event &event);

    void setSpawnValues(float value, SpawnQuantity toChange);

    void togglePause();

public:
    GameScene(sf::RenderWindow &_window, sf::Time &_targetFTime,
              sf::Time &_currentFTime, float &_currentFPS);
    virtual void update(sf::RenderWindow &window);
    virtual void load();
    void events(sf::Event &event);
    void redraw(sf::RenderWindow &window);
    virtual void unload();
};



#endif // CLASS_BALL_H
