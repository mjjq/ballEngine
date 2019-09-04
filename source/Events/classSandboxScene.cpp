#include "classSandboxScene.h"
#include "jsonParsing.h"

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

        ballSim = new BallUniverse{2000,2000,false,false};
        charMan = new CharacterManager{};
        projMan = new GameObjectManager{};
        objEditor = new GameObjectEditor{*projMan, window};
        skeletonMan = new Skeleton2DManager{};
        pSrcMan = new ParticleSystemManager{};
        //charWorldInterface = ICharWorld{ballSim, charMan, projMan};
        //ballSim->newObserver(&charWorldInterface);

        wSize = ballSim->getWorldSize();
        changeBoundaryRect(wSize);
        resetCamera();
        adjustViewSize(window.getSize());

        buttonFuncMap = {
            {"delObject",   [&]{objEditor->deleteObject();}},
            {"selObject",   [&]{objEditor->retrieveObject((sf::Vector2u)sf::Mouse::getPosition(window));}},
            {"mvObject",    [&]{objEditor->setObjectAttribute("position", window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                                objEditor->setObjectAttribute("velocity", sf::Vector2f{0.0f, 0.0f});
                                KeyBinds::isFuncContinuous = true;}},
            {"spwnMode",    [&]{switchControlMode("SpawnMode"); std::cout << "spawnmode\n";}},
            {"editMode",    [&]{switchControlMode("EditObjectMode"); std::cout << "editmode\n";}},
            {"charMode",    [&]{switchControlMode("CharacterMode"); std::cout << "charmode\n";}},
            {"incMass",     [&]{spawnMass+=1;}},
            {"decMass",     [&]{if(spawnMass>1){spawnMass-=1;}}},
            {"incRad",      [&]{spawnRadius+=1;}},
            {"decRad",      [&]{if(spawnRadius>1){spawnRadius-=1;}}},
            {"rstRad",      [&]{spawnRadius=10;}},
            {"rstMass",     [&]{spawnMass=1;}},
            {"setStar",     [&]{spawnRadius=50;spawnMass=10000;}},
            {"setPlanet",   [&]{spawnRadius=10;spawnMass=1;}},
            {"setAstrd",    [&]{spawnRadius=3;spawnMass=0.01;}},
            {"tglTrj",      [&]{ballSim->toggleTrajectories();}},
            {"tglPTraj",    [&]{ballSim->togglePlayerTraj();}},
            {"tglIntMthd",  [&]{ballSim->toggleRK4();}},
            {"clearSim",    [&]{projMan->clearAll();}},
            {"decSimStep",  [&]{if(dt > 0.1f) dt -=0.1f;}},
            {"incSimStep",  [&]{dt += 0.1f;}},
            {"zmToMse",     [&]{zoomToMouse(2.0f);}},
            {"zmFromMse",   [&]{zoomToMouse(0.5f);}},
            {"rstView",     [&]{resetCamera();}},
            {"tglSimPse",   [&]{isPaused = !isPaused;}},
            {"viewPan",     [&]{
                checkForViewPan(mousePosOnPan);
                canZoom = true;
                KeyBinds::isFuncContinuous = true;
                    }},
            {"aimChar",     [&]{
                charMan->setTarget(window.mapPixelToCoords(mousePosOnPan), 0);
                charMan->handleInput(Input::EnableTarget, 0);
                KeyBinds::isFuncContinuous = true;
                    }},
            {"focusPlr",    [&]{focusOnBall(playerBallIndex);}},
            {"pauseGme",    [&]{togglePause();}},
            {"tglForces",   [&]{ballSim->toggleForces();}},
            {"tglCols",     [&]{ballSim->toggleCollisions();}},
            {"tglGrav",     [&]{ballSim->toggleUGravity();}},
            {"chgBColour",  [&]{ballSim->changeBallColour();}},
            {"undoBall",    [&]{projMan->removeObject(-1);}},
            {"undoRect",    [&]{ballSim->removeRect(-1);}},
            {"newJoint",    [&]{ballSim->newJoint(0, window.mapPixelToCoords(sf::Mouse::getPosition(window)));}},
            {"mouseExp",    [&]{ballSim->createExplosion(window.mapPixelToCoords(sf::Mouse::getPosition(window)),
                                                        200.0f,
                                                        25.0f);}},
            {"mouseSin",    [&]{ballSim->createExplosion(window.mapPixelToCoords(sf::Mouse::getPosition(window)),
                                                        200.0f,
                                                        -1.0f);
                                KeyBinds::isFuncContinuous = true;}},
            {"equPrim",     [&]{charMan->handleInput(Input::Equip_Primary, 0);}},
            //{"nxtItem",     [&]{charMan->switchNextItem(0);}},
            {"plrJump",     [&]{charMan->handleInput(Input::Jump, 0);
                                KeyBinds::isFuncContinuous = false;}},
            {"mvPlrRgt",    [&]{charMan->handleInput(Input::WalkRight, 0);
                                KeyBinds::isFuncContinuous = true;}
                                },
            {"mvPlrLft",    [&]{charMan->handleInput(Input::WalkLeft, 0);
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
            {"mvPlrRgt",    [&]{charMan->handleInput(Input::Idle, 0);}
                                },
            {"mvPlrLft",    [&]{charMan->handleInput(Input::Idle, 0);}
                                },
            {"spwnSingle",  [&]{
                if(drawLine == true){
                    sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);

                    ObjectProperties props = {static_cast<sf::Vector2f>(mousePosOnClick),
                                             velocity,
                                             {spawnRadius, 0.0f},
                                             spawnMass,
                                             spawnCoefFriction,
                                             spawnCoefRest,
                                             spawnRotation,
                                             spawnRotRate,
                                             false, false, false, true,
                                             ObjectType::Ball,
                                             {},
                                             {"",
                                             "red.jpg",
                                             "normal2.png"}};
                    GameObject* obj = new GameObject(new Renderable(props),
                                                      new Polygon(props),
                                                     nullptr,
                                                     nullptr,
                                                     nullptr,
                                                     nullptr,
                                                     nullptr,
                                                     nullptr);//new ParticleSourceWrap(10000, 80.0f));
                    drawLine = false;
                }
            }
            },

            {"spwnChar",  [&]{
                if(drawLine == true){
                    /*sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);*/
                    /*charWorldInterface.spawnNewCharacter({static_cast<sf::Vector2f>(mousePosOnClick),
                                              velocity,
                                              {spawnRadius, 2.0f*spawnRadius},
                                              spawnMass,
                                              2.5f,
                                              9.0f
                                              });*/
                    ObjectProperties props = {static_cast<sf::Vector2f>(mousePosOnClick),
                                             {0.0f, 0.0f},
                                             {50.0f, 100.0f},
                                             spawnMass,
                                             spawnCoefFriction,
                                             spawnCoefRest,
                                             spawnRotation,
                                             spawnRotRate,
                                             false, false, false, true,
                                             ObjectType::Capsule,
                                             {},
                                             {"",
                                             "red.jpg",
                                             "normal2.png"}};
                    props._collisionGroup = {1,3};
                    CharacterProperties init;
                    GameObject* obj = new GameObject(nullptr, //new Renderable(props),
                                                        new Polygon(props),
                                                        nullptr,
                                                        new Character(init),
                                                        new Skeleton2DWrap("./res/json/example3.json"));
                    drawLine = false;
                }
            }
            },

            {"spwnCaps",  [&]{
                if(drawLine == true){
                    sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);
                    ballSim->spawnNewObject(
                                           {static_cast<sf::Vector2f>(mousePosOnClick),
                                         velocity,
                                         {spawnRadius, 3.0f*spawnRadius},
                                         spawnMass,
                                         spawnCoefFriction,
                                         spawnCoefRest,
                                         spawnRotation,
                                         spawnRotRate,
                                         false, false, false, true,
                                         ObjectType::Capsule});
                    drawLine = false;
                }
            }
            },

            {"spwnSCirc",  [&]{
                if(drawLine == true){
                    sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);
                    ObjectProperties props = {static_cast<sf::Vector2f>(mousePosOnClick),
                                             {0.0f, 0.0f},
                                             {500.0f*sqrtf(velocity.x*velocity.x+velocity.y*velocity.y), 0.0f},
                                             1e+15f,
                                             spawnCoefFriction,
                                             spawnCoefRest,
                                             spawnRotation,
                                             0.0f,
                                             true, false, false, true,
                                             ObjectType::Ball};

                    GameObject* obj = new GameObject(new Renderable(props),
                                                      new Polygon(props));
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

                    ObjectProperties props = {static_cast<sf::Vector2f>(mousePosOnClick),
                                             velocity,
                                             {spawnRadius, 0.0f},
                                             spawnMass,
                                             spawnCoefFriction,
                                             spawnCoefRest,
                                             spawnRotation,
                                             spawnRotRate,
                                             false, false, false, true,
                                             ObjectType::Polygon,
                                             verts,
                                             {"",
                                              "red.jpg",
                                              "pyramidN.png"}};
                    GameObject* obj = new GameObject(new Renderable(props),
                                                      new Polygon(props));
                    drawLine = false;
                }
            }
            },

            {"spwnJson",  [&]{
                if(drawLine == true){
                    sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);
                    //spawnFromJson(static_cast<sf::Vector2f>(mousePosOnClick),velocity);

                    ObjectProperties props;
                    props._position = (sf::Vector2f )(mousePosOnClick);
                    props._velocity = velocity;
                    nlohmann::json j = beParser::loadJsonFromFile("./res/json/square.json");
                    beParser::checkObjectPropertyParams(j, props);

                    GameObject* obj = new GameObject(new Renderable(props),
                                                     new Polygon(props));

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
                        vert.position = Math::rotate(vert.position, spawnRotation);
                    }
                    ObjectProperties props = {static_cast<sf::Vector2f>(mousePosOnClick),
                                             {0.0f, 0.0f},
                                             {0.0f, 0.0f},
                                             1e+15f,
                                             spawnCoefFriction,
                                             spawnCoefRest,
                                             0.0f,
                                             0.0f,
                                             true, false, false, true,
                                             ObjectType::Polygon,
                                             verts};

                    GameObject* obj = new GameObject(new Renderable(props),
                                                      new Polygon(props));
                    drawLine = false;
                }
            }
            },

            {"spwnDPoly",  [&]{
                if(drawLine == true){
                    sf::Vector2f velocity = velocityFromMouse(mousePosOnClick,
                                                              spawnVelFactor);
                    std::vector<sf::Vertex > verts = {
  {{0.15f, -3.65f}},
{{2.475f, -3.275f}},
{{1.15f, -3.175f}},
{{1.0f, -2.175f}},
{{2.325f, -1.775f}},
{{4.725f, -1.725f}},
{{6.225f, -1.175f}},
{{6.95f, -0.05f}},
{{6.625f, 1.775f}},
{{4.775f, 4.15f}},
{{3.4f, 4.275f}},
{{2.425f, 3.175f}},
{{2.575f, 1.925f}},
{{1.825f, 0.0f}},
{{1.05f, -0.15f}},
{{-2.225f, 1.175f}},
{{-3.4f, 0.3f}},
{{-3.95f, -1.25f}},
{{-3.35f, -2.5f}},
{{-1.05f, -3.625f}}
                    };

                    for(sf::Vertex &vert : verts)
                    {
                        vert.position.x = vert.position.x * spawnRadius*5.0f;
                        vert.position.y = vert.position.y * spawnRadius*5.0f;
                    }
                    ObjectProperties props = {static_cast<sf::Vector2f>(mousePosOnClick),
                                             velocity,
                                             {spawnRadius, 0.0f},
                                             spawnMass,
                                             spawnCoefFriction,
                                             spawnCoefRest,
                                             spawnRotation,
                                             spawnRotRate,
                                             false, false, false, true,
                                             ObjectType::ConcavePoly,
                                             verts,
                                             {"",
                                             "",
                                             "normal.jpg",
                                             "spiro.jpg"}};
                    GameObject* obj = new GameObject(new Renderable(props),
                                                      new Polygon(props));
                    drawLine = false;
                }
            }
            },

            {"spwnLight",  [&]{
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

                    for(sf::Vertex &vert : verts)
                    {
                        vert.position.x = vert.position.x * spawnRadius/10.0f;
                        vert.position.y = vert.position.y * spawnRadius/10.0f;
                    }
                    ObjectProperties props = {static_cast<sf::Vector2f>(mousePosOnClick),
                                             velocity,
                                             {spawnRadius, 0.0f},
                                             spawnMass,
                                             spawnCoefFriction,
                                             spawnCoefRest,
                                             spawnRotation,
                                             spawnRotRate,
                                             false, false, false, true,
                                             ObjectType::Ball,
                                             verts,
                                             {"light.frag"},
                                             };
                    LightProperties lProperties = {{props._position.x, props._position.y, 5.0f}};
                    lProperties.umbralRadius = spawnRadius;
                    GameObject* obj = new GameObject(new Renderable(props),
                                                      new Polygon(props),
                                                      new LightSource(lProperties));
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
                    ObjectProperties props = {static_cast<sf::Vector2f>(mousePosOnClick),
                                            {0.0f, 0.0f},
                                            {0.0f, 0.0f},
                                            spawnMass,
                                             spawnCoefFriction,
                                             spawnCoefRest,
                                             spawnRotation,
                                             spawnRotRate,
                                             true, false, false, true,
                                             ObjectType::Polygon,
                                             verts,
                                             {"",
                                             "red.jpg",
                                             "normal.jpg",
                                             "spiro.jpg"}};
                    GameObject* obj = new GameObject(new Renderable(props),
                                                      new Polygon(props));
                    drawLine = false;
                }
            }
            },

            {"aimChar",     [&]{
                charMan->handleInput(Input::DisableTarget, 0);
            }},
        };

        sliderFuncMap = {
            {"changeMass",  {[&](float mass){setSpawnValues(mass,SQ_MASS);}, &spawnMass}},
            {"changeRad",   {[&](float radius){setSpawnValues(radius, SQ_RADIUS);}, &spawnRadius}},
            {"changeRot",   {[&](float rotation){setSpawnValues(rotation, SQ_ROTATION);}, &spawnRotation}},
            {"changeRR",    {[&](float rotRate){setSpawnValues(rotRate, SQ_ROTRATE);}, &spawnRotRate}},
            {"changeCR",    {[&](float coefRest){setSpawnValues(coefRest, SQ_CREST);}, &spawnCoefRest}},
            {"changeCF",    {[&](float coefFric){setSpawnValues(coefFric, SQ_CFRICTION);}, &spawnCoefFriction}}
        };

        textVarMap = {
            {"numBalls",    [&]{return ballSim->getNumOfBalls();}},
            {"timeStep",    [&]{return std::to_string(dt);}},
            {"spawnMass",   [&]{return std::to_string(spawnMass);}},
            {"spawnRad",    [&]{return std::to_string(spawnRadius);}},
            {"spawnRot",    [&]{return std::to_string(spawnRotation);}},
            {"spawnRR",     [&]{return std::to_string(spawnRotRate);}},
            {"spawnCR",     [&]{return std::to_string(spawnCoefRest);}},
            {"spawnCF",     [&]{return std::to_string(spawnCoefFriction);}},
            {"forceEnld",   [&]{return ballSim->getForcesEnabled();}},
            {"collsEnld",   [&]{return ballSim->getCollisionsEnabled();}},
            {"gravEnld",    [&]{return ballSim->getUGravityEnabled();}},
            {"totalKE",     [&]{return ballSim->getTotalKE();}},
            {"totalMom",    [&]{return ballSim->getTotalMomentum();}},
            {"totalEngy",   [&]{return ballSim->getTotalEnergy();}},
            {"intMthd",     [&]{return ballSim->getUseRK4();}},
            {"currFPS",     [&]{return std::to_string(currentFPS);}}
        };

        loadUI("sandboxsceneUI.json", container);
        loadKeybinds("keybindsSandbox.json", "SpawnMode");

        Collisions::setDebugWindow(window);


        std::vector<sf::Vertex > verts = {
            sf::Vertex{{0.0f, 0.0f}},
            sf::Vertex{{(float)wSize.x, 0.0f}},
            sf::Vertex{{(float)wSize.x, (float)wSize.y}},
            sf::Vertex{{0.0f, (float)wSize.y}}
        };
        ObjectProperties props = {{(float)wSize.x/2.0f, (float)wSize.y/2.0f},
                                 {0.0f, 0.0f},
                                 {1.0f, 1.0f},
                                 spawnMass,
                                 spawnCoefFriction,
                                 spawnCoefRest,
                                 spawnRotation,
                                 spawnRotRate,
                                 false, false, false, true,
                                 ObjectType::Polygon,
                                 verts,
                                 {"phong",
                                 "white.png",
                                 "blankN.jpg"}
                                             };
        //projMan->addObject(new GameObject(new Renderable(props)));
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

    if(!isPaused)
    {
        float factor = 1.0f;
        float minFramerate = 10.0f;

        if(currentFrameTime.asSeconds() > 1.0f/minFramerate)
            accumulator += factor*120*dt*targetFrameTime.asSeconds();
        else
            accumulator += factor*120*dt*currentFrameTime.asSeconds();

        while(accumulator >= dt)
        {
            thresholdTimer.restart();


            skeletonMan->updateAll(0.007f*dt/factor);
            pSrcMan->updateAll(1.0f*dt/factor);
            ballSim->physicsLoop(dt/factor);


            accumulator -= dt;
        }
    }

    timeToNextSpawn -= currentFrameTime;
}
