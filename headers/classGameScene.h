#ifndef CLASS_GAMESCENE_H
#define CLASS_GAMESCENE_H


#include "classScene.h"
#include "classUniverse.h"
#include "classCharacterManager.h"
#include "classGameObjectManager.h"
#include "classGameObjectEditor.h"
#include "characterWorldInterface.h"
#include "classUIContainer.h"
#include "classSkeleton2DManager.h"
#include "ParticleSystemManager.h"
#include "Camera.h"

class GameScene : public Scene
{
protected:
    enum SpawnQuantity
    {
        SQ_MASS,
        SQ_RADIUS,
        SQ_ROTATION,
        SQ_ROTRATE,
        SQ_CREST,
        SQ_CFRICTION,
        SQ_DENSITY,
    };

    Camera camera{window, worldView};

    sf::Vector2i mousePosOnClick;
    sf::Vector2i mousePosOnPan;
    sf::Vector2i mousePosOnRelease;
    bool drawLine = false;

    //void adjustViewSize(sf::Vector2u newSize);//, float zoom);

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
    float spawnRotation = 0.0f;
    float spawnRotRate = 0.0f;
    float spawnCoefRest = 0.0f;
    float spawnCoefFriction = 1.0f;

    float dt = 1.0f;
    float accumulator = 0.0f;
    sf::Clock thresholdTimer;
    bool isPaused = false;

    BallUniverse* ballSim = nullptr;//{2000, 2000, 1.0f, false, false};
    CharacterManager* charMan = nullptr;
    GameObjectManager* projMan = nullptr;
    GameObjectEditor* objEditor = nullptr;
    Skeleton2DManager* skeletonMan = nullptr;
    ParticleSystemManager* pSrcMan = nullptr;
    //ICharWorld charWorldInterface{nullptr, nullptr, nullptr};
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
