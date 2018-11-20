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
                KeyBinds::isFuncContinuous = true;
                    }},
            {"setPlyr0",    [&]{ballSim.setPlayer(0);}},
            {"setPlyr1",    [&]{ballSim.setPlayer(1);}},
            {"focusPlr",    [&]{focusOnBall(playerBallIndex);}},
            {"pauseGme",    [&]{togglePause();}},
            {"tglForces",   [&]{ballSim.toggleForces();}},
            {"tglCols",     [&]{ballSim.toggleCollisions();}},
            {"tglGrav",     [&]{ballSim.toggleUGravity();}},
            {"chgBColour",  [&]{ballSim.changeBallColour();}},
            {"undoBall",    [&]{ballSim.removeBall(-1);}},
            {"undoRect",    [&]{ballSim.removeRect(-1);}},
            {"mvPlrFwd",    [&]{ballSim.playerInFunc({0,1});
                                KeyBinds::isFuncContinuous = true;}},
            {"mvPlrRgt",    [&]{ballSim.playerInFunc({-1,0});
                                KeyBinds::isFuncContinuous = true;}
                                },
            {"mvPlrBck",    [&]{ballSim.playerInFunc({0,-1});
                                KeyBinds::isFuncContinuous = true;}
                                },
            {"mvPlrLft",    [&]{ballSim.playerInFunc({1,0});
                                KeyBinds::isFuncContinuous = true;}
                                },
            {"spwnSingle",  [&]{
                if(!mouseOnUIWhenClicked.first){
                    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
                    mousePosOnClick = static_cast<sf::Vector2i>
                    (window.mapPixelToCoords(viewPos));
                    drawLine = true;
                }
                }
            },
            {"spwnDAABB",  [&]{
                if(!mouseOnUIWhenClicked.first){
                    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
                    mousePosOnClick = static_cast<sf::Vector2i>
                    (window.mapPixelToCoords(viewPos));
                    drawLine = true;
                }
                }
            },
            {"spwnSCirc",  [&]{
                if(!mouseOnUIWhenClicked.first){
                    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
                    mousePosOnClick = static_cast<sf::Vector2i>
                    (window.mapPixelToCoords(viewPos));
                    drawLine = true;
                }
                }
            },
            {"spwnJson",  [&]{
                if(!mouseOnUIWhenClicked.first){
                    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
                    mousePosOnClick = static_cast<sf::Vector2i>
                    (window.mapPixelToCoords(viewPos));
                    drawLine = true;
                }
                }
            },
            {"spwnSys",  [&]{
                if(!mouseOnUIWhenClicked.first){
                    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
                    mousePosOnClick = static_cast<sf::Vector2i>
                    (window.mapPixelToCoords(viewPos));
                    drawLine = true;
                }
                }
            },
            {"spwnDPoly",  [&]{
                if(!mouseOnUIWhenClicked.first){
                    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
                    mousePosOnClick = static_cast<sf::Vector2i>
                    (window.mapPixelToCoords(viewPos));
                    drawLine = true;
                }
                }
            },
            {"spwnSPoly",  [&]{
                if(!mouseOnUIWhenClicked.first){
                    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
                    mousePosOnClick = static_cast<sf::Vector2i>
                    (window.mapPixelToCoords(viewPos));
                    drawLine = true;
                }
                }
            },
        };
        buttonReleaseMap = {
            {"spwnSingle",  [&]{
                if(drawLine == true){
                    sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);
                    ballSim.spawnNewBall({static_cast<sf::Vector2f>(mousePosOnClick),
                                         velocity,
                                         {spawnRadius, 0.0f},
                                         spawnMass,
                                         1.0f,
                                         0.0f,
                                         spawnRotation,
                                         0.0f});
                    drawLine = false;
                }
            }
            },

            {"spwnSCirc",  [&]{
                if(drawLine == true){
                    sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);
                    ballSim.spawnStaticBall({static_cast<sf::Vector2f>(mousePosOnClick),
                                            {0.0f, 0.0f},
                                            {300.0f*velocity.x, 300.0f*velocity.y},
                                            0.0f,
                                            1.0f,
                                            0.0f,
                                            spawnRotation,
                                            0.0f,
                                            });
                    drawLine = false;
                }
            }
            },

            {"spwnDAABB",  [&]{
                if(drawLine == true){
                    sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);
                    std::vector<sf::Vertex > verts = {
                        sf::Vertex{{-5.0f, -5.0f}},
                        sf::Vertex{{5.0f, -5.0f}},
                        sf::Vertex{{5.0f, 5.0f}},
                        sf::Vertex{{-5.0f, 5.0f}}
                    };
                    for(sf::Vertex &vert : verts)
                        vert.position = vert.position*spawnRadius;
                    ballSim.spawnNewPoly(verts,
                                         {static_cast<sf::Vector2f>(mousePosOnClick),
                                         velocity,
                                         {0.0f, 0.0f},
                                         spawnMass,
                                         1.0f, 0.0f,
                                         spawnRotation,
                                         0.0f});
                    drawLine = false;
                }
            }
            },

            {"spwnJson",  [&]{
                if(drawLine == true){
                    sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);
                    spawnFromJson(static_cast<sf::Vector2f>(mousePosOnClick),velocity);
                    drawLine = false;
                }
            }
            },

            {"spwnSys",  [&]{
                if(drawLine == true){
                    sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);
                    std::vector<sf::Vertex > verts = {
                        sf::Vertex{{-10.0f, -10.0f}},
                        sf::Vertex{{10.0f, -10.0f}},
                        sf::Vertex{{10.0f, 10.0f}},
                        sf::Vertex{{-10.0f, 10.0f}}
                    };
                    for(sf::Vertex &vert : verts)
                    {
                        vert.position.x = 30.0f * vert.position.x * velocity.x;
                        vert.position.y = 30.0f * vert.position.y * velocity.y;
                    }
                    ballSim.spawnStaticPoly(verts,
                                         {static_cast<sf::Vector2f>(mousePosOnClick),
                                            {0.0f, 0.0f},
                                            {0.0f, 0.0f},
                                            0.0f,
                                            1.0f,
                                            0.0f,
                                             spawnRotation,
                                             0.0f});
                    drawLine = false;
                }
            }
            },

            {"spwnDPoly",  [&]{
                if(drawLine == true){
                    sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);
                    std::vector<sf::Vertex > verts = {
                        sf::Vertex{{-10.0f, -35.0f}},
                        sf::Vertex{{10.0f, -10.0f}},
                        sf::Vertex{{10.0f, 10.0f}},
                        sf::Vertex{{-10.0f, 10.0f}},
                        sf::Vertex{{-30.0f, 0.0f}},
                        sf::Vertex{{-30.0f, -10.0f}}
                    };
                    /*std::vector<sf::Vertex > verts = {
                        sf::Vertex{{-10.0f, -10.0f}},
                        sf::Vertex{{20.0f, -10.0f}},
                        sf::Vertex{{20.0f, 10.0f}},
                        sf::Vertex{{-10.0f, 10.0f}}
                    };*/
                    ballSim.spawnNewPoly(verts,
                                         {static_cast<sf::Vector2f>(mousePosOnClick),
                                         velocity,
                                         {0.0f, 0.0f},
                                         spawnMass,
                                         1.0f, 0.0f,
                                         spawnRotation,
                                         0.0f});
                    drawLine = false;
                }
            }
            },

            {"spwnSPoly",  [&]{
                if(drawLine == true){
                    sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);
                    std::vector<sf::Vertex > verts = {
                        sf::Vertex{{-10.0f, -35.0f}},
                        sf::Vertex{{10.0f, -10.0f}},
                        sf::Vertex{{10.0f, 10.0f}},
                        sf::Vertex{{-10.0f, 10.0f}},
                        sf::Vertex{{-30.0f, 0.0f}},
                        sf::Vertex{{-30.0f, -10.0f}}
                    };
                    /*std::vector<sf::Vertex > verts = {
                        sf::Vertex{{-10.0f, -10.0f}},
                        sf::Vertex{{10.0f, -10.0f}},
                        sf::Vertex{{10.0f, 10.0f}},
                        sf::Vertex{{-10.0f, 10.0f}}
                    };*/
                    for(sf::Vertex &vert : verts)
                    {
                        vert.position = vert.position * velocity.x*velocity.y;
                    }
                    ballSim.spawnStaticPoly(verts,
                                         {static_cast<sf::Vector2f>(mousePosOnClick),
                                            {0.0f, 0.0f},
                                            {0.0f, 0.0f},
                                            0.0f,
                                            1.0f, 0.0f,
                                            spawnRotation, 0.0f});
                    drawLine = false;
                }
            }
            }
        };

        sliderFuncMap = {
            {"changeMass",  {[&](float mass){setSpawnValues(mass,SQ_MASS);}, &spawnMass}},
            {"changeRad",   {[&](float radius){setSpawnValues(radius, SQ_RADIUS);}, &spawnRadius}},
            {"changeRot",   {[&](float rotation){setSpawnValues(rotation, SQ_ROTATION);}, &spawnRotation}}
        };

        textVarMap = {
            {"numBalls",    [&]{return ballSim.getNumOfBalls();}},
            {"timeStep",    [&]{return ballSim.getTimeStep();}},
            {"spawnMass",   [&]{return std::to_string(spawnMass);}},
            {"spawnRad",    [&]{return std::to_string(spawnRadius);}},
            {"spawnRot",    [&]{return std::to_string(spawnRotation);}},
            {"forceEnld",   [&]{return ballSim.getForcesEnabled();}},
            {"collsEnld",   [&]{return ballSim.getCollisionsEnabled();}},
            {"gravEnld",    [&]{return ballSim.getUGravityEnabled();}},
            {"totalKE",     [&]{return ballSim.getTotalKE();}},
            {"totalMom",    [&]{return ballSim.getTotalMomentum();}},
            {"totalEngy",   [&]{return ballSim.getTotalEnergy();}},
            {"intMthd",     [&]{return ballSim.getUseRK4();}},
            {"currFPS",     [&]{return std::to_string(currentFPS);}}
        };

        loadUI("./json/sandboxsceneUI.json", container);
        loadKeybinds("./json/keybinds.json", "SandboxScene");

        Collisions::setDebugWindow(window);
        std::cout << &window << "\n";
    }
}

void SandboxScene::update(sf::RenderWindow &_window)
{
    if(!mouseOnUIWhenClicked.first)
    {
        checkMBPress(mousePosOnClick,drawLine);
    }
    if(clickedWindowToDrag)
        container.dragWindow(_window);

    window.setMouseCursorVisible(true);

    KeyBinds::exePressedKeys(pressedKeyStack, keyBinds);
    KeyBinds::exeReleasedKey(pressedKeyStack, releasedKeyStack, releasedKeyBinds);

    mousePosOnPan = sf::Mouse::getPosition(window);

    ballSim.universeLoop(currentFrameTime, targetFrameTime);

    timeToNextSpawn -= currentFrameTime;
}
