#include "../../headers/classSurvivalScene.h"

SurvivalScene::SurvivalScene(sf::RenderWindow &_window,
                           sf::Time &_targetFTime,
                           sf::Time &_currentFTime,
                           float &_currentFPS) :
                           GameScene(_window, _targetFTime, _currentFTime, _currentFPS)
{

}

void SurvivalScene::mouseWorldEvents(sf::Event &event) {}

void SurvivalScene::load()
{
    if(!isLoaded)
    {
        isLoaded = true;
        wSize = ballSim.getWorldSize();
        changeBoundaryRect(wSize);
        resetCamera();
        adjustViewSize(window.getSize());

        buttonFuncMap = {
            {"incMass",     [&]{spawnMass+=1;}},
            {"decMass",     [&]{if(spawnMass>1){spawnMass-=1;}}},
            {"incRad",      [&]{spawnRadius+=1;}},
            {"decRad",      [&]{if(spawnRadius>1){spawnRadius-=1;}}},
            {"rstRad",      [&]{spawnRadius=10;}},
            {"rstMass",     [&]{spawnMass=1;}},
            {"setStar",     [&]{spawnRadius=50;spawnMass=10000;}},
            {"setPlanet",   [&]{spawnRadius=10;spawnMass=1;}},
            {"setAstrd",    [&]{spawnRadius=3;spawnMass=0.01;}},
            {"tglTrj",      [&]{ballSim.toggleTrajectories();}},
            {"tglPTraj",    [&]{ballSim.togglePlayerTraj();}},
            {"tglIntMthd",  [&]{ballSim.toggleRK4();}},
            {"clearSim",    [&]{ballSim.clearSimulation();}},
            {"decSimStep",  [&]{ballSim.decSimStep(0.1);}},
            {"incSimStep",  [&]{ballSim.incSimStep(0.1);}},
            {"zmToMse",     [&]{zoomToMouse(2.0f);}},
            {"zmFromMse",   [&]{zoomToMouse(0.5f);}},
            {"rstView",     [&]{resetCamera();}},
            {"tglSimPse",   [&]{ballSim.toggleSimPause();}},
            {"viewPan",     [&]{
                checkForViewPan(mousePosOnPan);
                canZoom = true;
                    }},
            {"setPlyr0",    [&]{ballSim.setPlayer(0);}},
            {"setPlyr1",    [&]{ballSim.setPlayer(1);}},
            {"focusPlr",    [&]{focusOnBall(playerBallIndex);}},
            {"pauseGme",    [&]{togglePause();}},
            {"tglForces",   [&]{ballSim.toggleForces();}},
            {"tglCols",     [&]{ballSim.toggleCollisions();}},
            {"chgBColour",  [&]{ballSim.changeBallColour();}},
            {"undoBall",    [&]{ballSim.removeBall(-1);}},
            {"undoRect",    [&]{ballSim.removeRect(-1);}},
            {"mvPlrFwd",    [&]{ballSim.playerInFunc({0,1});}},
            {"mvPlrRgt",    [&]{ballSim.playerInFunc({-1,0});}},
            {"mvPlrBck",    [&]{ballSim.playerInFunc({0,-1});}},
            {"mvPlrLft",    [&]{ballSim.playerInFunc({1,0});}},
            {"startGme",    [&]{startTheGame = true;}},
        };

        sliderFuncMap = {
        };

        textVarMap = {
            {"currFPS",     [&]{return std::to_string(currentFPS);}}
        };

        loadUI("./json/SurvivalSceneUI.json", container);
        loadKeybinds("./json/keybinds.json", "SurvivalScene");

        spawnFromJson({wSize.x/2.0f, wSize.y/2.0f}, {2,0});
        ballSim.setPlayer(0);
        ballSim.toggleCollisions();
        ballSim.toggleTrajectories();
        ballSim.togglePlayerTraj();
    }
}

void SurvivalScene::update(sf::RenderWindow &_window)
{
    exePressedKeys();

    if(clickedWindowToDrag)
        container.dragWindow(_window);

    window.setMouseCursorVisible(false);

    mousePosOnPan = sf::Mouse::getPosition(window);

    if(startTheGame)
    {
        if(countDownTimer.asSeconds() <= 0.0f)
        {
            ballSim.universeLoop(currentFrameTime, targetFrameTime);

            timeToNextSpawn -= currentFrameTime;
        }
        else
            countDownTimer -= currentFrameTime;
    }
}
