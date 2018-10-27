#include "../../headers/classSandboxScene.h"

SandboxScene::SandboxScene(sf::RenderWindow &_window,
                           sf::Time &_targetFTime,
                           sf::Time &_currentFTime,
                           float &_currentFPS) :
                           GameScene(_window, _targetFTime, _currentFTime, _currentFPS)
{

}

void SandboxScene::load()
{
    if(!isLoaded)
    {
        isLoaded = true;

        ballSim = BallUniverse{2000,2000,1.0f,false,false};

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
        };

        sliderFuncMap = {
            {"changeMass",  {[&](float mass){setSpawnValues(mass,SQ_MASS);}, &spawnMass}},
            {"changeRad",   {[&](float radius){setSpawnValues(radius, SQ_RADIUS);}, &spawnRadius}}
        };

        textVarMap = {
            {"numBalls",    [&]{return ballSim.getNumOfBalls();}},
            {"timeStep",    [&]{return ballSim.getTimeStep();}},
            {"spawnMass",   [&]{return std::to_string(spawnMass);}},
            {"spawnRad",    [&]{return std::to_string(spawnRadius);}},
            {"forceEnld",   [&]{return ballSim.getForcesEnabled();}},
            {"collsEnld",   [&]{return ballSim.getCollisionsEnabled();}},
            {"totalKE",     [&]{return ballSim.getTotalKE();}},
            {"totalMom",    [&]{return ballSim.getTotalMomentum();}},
            {"totalEngy",   [&]{return ballSim.getTotalEnergy();}},
            {"intMthd",     [&]{return ballSim.getUseRK4();}},
            {"currFPS",     [&]{return std::to_string(currentFPS);}}
        };

        loadUI("./json/sandboxsceneUI.json", container);
        loadKeybinds("./json/keybinds.json", "SandboxScene");

    }
}

void SandboxScene::update(sf::RenderWindow &_window)
{
    if(!mouseOnUIWhenClicked.first)
    {
        checkMBPress(mousePosOnClick,sf::Mouse::isButtonPressed(sf::Mouse::Left));
        checkMBPress(mousePosOnClick,sf::Mouse::isButtonPressed(sf::Mouse::Middle));
        checkMBPress(mousePosOnClick,sf::Mouse::isButtonPressed(sf::Mouse::Right));
    }
    if(clickedWindowToDrag)
        container.dragWindow(_window);

    window.setMouseCursorVisible(true);

    KeyBinds::exePressedKeys(pressedKeyStack, keyBinds);

    mousePosOnPan = sf::Mouse::getPosition(window);

    ballSim.universeLoop(currentFrameTime, targetFrameTime);

    timeToNextSpawn -= currentFrameTime;
}
