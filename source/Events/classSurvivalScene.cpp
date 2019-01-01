#include "classSurvivalScene.h"

SurvivalScene::SurvivalScene(sf::RenderWindow &_window,
                           sf::Time &_targetFTime,
                           sf::Time &_currentFTime,
                           float &_currentFPS) :
                           GameScene(_window, _targetFTime, _currentFTime, _currentFPS)
{

}

void SurvivalScene::mouseWorldEvents(sf::Event &event) {}

void SurvivalScene::spawnRandomBalls(int nOBalls,
                          sf::Vector2f position,
                          float boundingRadius,
                          float ballRadius,
                          float ballMass,
                          float maxSpeed)
{
    std::srand(2322);

    for(int i=0; i<nOBalls; ++i)
    {
        float relativePosX = ((std::rand()-RAND_MAX/2.0f)/RAND_MAX)*boundingRadius;
        float relativePosY = ((std::rand()-RAND_MAX/2.0f)/RAND_MAX)*boundingRadius;
        float velocityX = ( (std::rand()-RAND_MAX/2.0f)/RAND_MAX )*maxSpeed;

        float plusMinusOne = 1.0f;
        if(std::rand()-RAND_MAX/2.0f < 0)
            plusMinusOne = -1.0f;

        float velocityY = plusMinusOne*sqrt(maxSpeed*maxSpeed - velocityX*velocityX);

        sf::Vector2f newPos = {relativePosX+position.x, relativePosY+position.y};
        sf::Vector2f newVelocity = {velocityX, velocityY};

        ballSim.spawnNewBall({newPos,
                             newVelocity,
                             {ballRadius, 0.0f},
                             ballMass,
                             0.0f,
                             1.0f,
                             0.0f,
                             0.0f});
    }
}

void SurvivalScene::startGame()
{
    if(gameLogic.startTheGame == false)
    {
        gameLogic.startTheGame = true;
        container.setWindowIsVisible(0, false);
        container.setWindowIsVisible(1, true);
    }
}

void SurvivalScene::endGame()
{
    if(gameLogic.gameOver == false)
    {
        gameLogic.gameOver = true;
        gameLogic.allowPlayerMovement = false;
        //container.setWindowIsVisible(2, false);
        container.setWindowIsVisible(3, true);
    }
}

void SurvivalScene::restartGame()
{
    if(gameLogic.gameOver == true)
    {
        unload();
        load();
    }
}

void SurvivalScene::load()
{
    if(!isLoaded)
    {
        isLoaded = true;

        ballSim = BallUniverse{2000, 2000, 1.0f, false, false};

        gameLogic = GameLogic{};

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
            {"focusPlr",    [&]{focusOnBall(playerBallIndex);}},
            {"pauseGme",    [&]{togglePause();}},
            {"tglForces",   [&]{ballSim.toggleForces();}},
            {"tglCols",     [&]{ballSim.toggleCollisions();}},
            {"chgBColour",  [&]{ballSim.changeBallColour();}},
            {"undoBall",    [&]{ballSim.removeBall(-1);}},
            {"undoRect",    [&]{ballSim.removeRect(-1);}},
            //{"mvPlrFwd",    [&]{ballSim.playerInFunc({0,1});
            //                    KeyBinds::isFuncContinuous = true;}},
            /*{"mvPlrRgt",    [&]{if(gameLogic.allowPlayerMovement)ballSim.playerInFunc({-0.2,0});
                                KeyBinds::isFuncContinuous = true;}},
            {"mvPlrBck",    [&]{ballSim.playerInFunc({0,-1});
                                KeyBinds::isFuncContinuous = true;}},
            {"mvPlrLft",    [&]{if(gameLogic.allowPlayerMovement)ballSim.playerInFunc({0.2,0});
                                KeyBinds::isFuncContinuous = true;}},*/
            {"startGme",    [&]{startGame();}},
            {"restart",     [&]{restartGame();}}
        };

        sliderFuncMap = {
        };

        textVarMap = {
            {"currFPS",     [&]{return std::to_string(currentFPS);}},
            {"cdtimer",     [&]{return std::to_string(1+gameLogic.countDownTimer.asMilliseconds()/1000);}},
            {"uptimer",     [&]{return std::to_string(gameLogic.upTimer.asMilliseconds());}},
            {"playSpd",     [&]{return ballSim.getBallSpeed(playerBallIndex);}}
        };

        loadKeybinds("./json/keybinds.json", "SurvivalScene");
        loadUI("./json/survivalsceneUI.json", container);

        container.setWindowIsVisible(1, false);
        container.setWindowIsVisible(2, false);
        container.setWindowIsVisible(3, false);

        ballSim.spawnNewBall({sf::Vector2f{wSize.x/2.0f, wSize.y/1.2f},
                             {0.0f, 2.0f},
                             {10.0f, 0.0f},
                             2.0f,
                             1.0f,
                             0.0f,
                             0.0f,
                             0.0f});
        spawnRandomBalls(50, sf::Vector2f{wSize.x/2.0f, wSize.y/2.0f}, wSize.x/2.0f, 10.0f, 2.0f, 2.0f);
        //spawnFromJson({wSize.x/2.0f, wSize.y/2.0f}, {3,0});
        //ballSim.setPlayer(0);
        ballSim.toggleCollisions();
        ballSim.toggleTrajectories();
        ballSim.togglePlayerTraj();
    }
}

void SurvivalScene::update(sf::RenderWindow &_window)
{
    KeyBinds::exePressedKeys(pressedKeyStack, keyBinds);

    if(clickedWindowToDrag)
        container.dragWindow(_window);

    window.setMouseCursorVisible(false);

    mousePosOnPan = sf::Mouse::getPosition(window);

    if(gameLogic.startTheGame && !gameLogic.gameOver)
    {
        if(gameLogic.countDownTimer.asSeconds() <= 0.0f)
        {
            ballSim.universeLoop(currentFrameTime, targetFrameTime);

            gameLogic.upTimer += currentFrameTime;

            if(ballSim.getNumTimesColld(playerBallIndex) > 0)
                endGame();
        }
        else
        {
            gameLogic.countDownTimer -= currentFrameTime;
            if(gameLogic.countDownTimer.asSeconds() <= 0.0f)
            {
                gameLogic.allowPlayerMovement = true;
                container.setWindowIsVisible(1, false);
                container.setWindowIsVisible(2, true);
            }
        }
    }
    else if(gameLogic.gameOver)
    {
    }
}
