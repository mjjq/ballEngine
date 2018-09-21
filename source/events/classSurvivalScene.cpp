/**
    @class NonSimulationStuff
    Purpose: Handle all aspects of the program which are not involved in
    simulation. This includes event handling of user inputs, and functions
    to do with rendering.

    @author mjjq
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>
#include <numeric>
#include <fstream>
#include <map>
#include "../../extern/json.hpp"

#include "../../headers/classSurvivalScene.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"
#include "../../headers/jsonParsing.h"




/**
    Generates a spawn velocity for a new ball proportional to the distance
    between the mouse position on click and the current mouse position.

    @param mousePosOnClick The mouse position when a chosen mouse button was
    clicked.
    @param spawnVelFactor The factor by which the aforementioned distance is
    multiplied. A larger value will generate a larger spawn velocity.

    @return The calculated velocity.
*/
sf::Vector2f SurvivalScene::velocityFromMouse(sf::Vector2i _mousePosOnClick,
                                                            int _spawnVelFactor)
{

    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
    sf::Vector2i mPosOnRelease = static_cast<sf::Vector2i>
                                     (window.mapPixelToCoords(viewPos));

    sf::Vector2i scaledVector = (_spawnVelFactor)*(mPosOnRelease-
                                                  _mousePosOnClick);

    /*float windowDiagSize = pow(windowSizeX*windowSizeX +
                               windowSizeY*windowSizeY, 0.5);*/
    float worldDiagSize = sqrt(sfVectorMath::dot( ballSim.getWorldSize(),
                                                  ballSim.getWorldSize() ));

    sf::Vector2f velocity = static_cast<sf::Vector2f>(scaledVector)/worldDiagSize;
    timeToNextSpawn = minTimeToNextSpawn;

    return velocity;
}

/**
    Applies a zoom to the rendered view, and centres the view to the position
    of the mouse. The if statement imposes a maximum zoom out limit.

    @param zoomFactor The factor by which to zoom the current view.
*/
void SurvivalScene::zoomToMouse(float zoomFactor)
{
    if(currentZoom*zoomFactor < 2.0)
    {
        currentZoom *= zoomFactor;
        worldView.zoom(zoomFactor);
        window.setView(worldView);
    }
}


/**
    Generates effective zooms in the x and y directions. The effective zoom
    is the zoom factor in world co-ordinates as opposed to window co-ordinates,
    and so will differ in the x and y directions if the window is not square
    but rectangular.
    If the simulation is set to not fit within the window (simFitsInWindow==
    false), then the window and world are 1:1 in co-ordinates, thus the
    effective zoom is 1.

    @param worldSizeX The x-direction size of the simulation world.
    @param worldSizeY The y-direction size of the simulation world.

    @return The effective world zoom of each direction in vector form.
*/
sf::Vector2f SurvivalScene::getEffectiveZoom(int worldSizeX, int worldSizeY)
{
    sf::Vector2f effectiveZoom;
    if(simFitsInWindow)
    {
        float zoom = currentZoom;
        float winSizeX = window.getSize().x;
        float winSizeY = window.getSize().y;
        //sf::Vector2i viewPos = sf::Mouse::getPosition(window);
        float xFactor = zoom*static_cast<float>(worldSizeX)/
                             static_cast<float>(winSizeX);
        float yFactor = zoom*static_cast<float>(worldSizeY)/
                             static_cast<float>(winSizeY);
        effectiveZoom = {xFactor,yFactor};
    }
    else
        effectiveZoom = {1,1};
    return effectiveZoom;
}

/**
    Checks if a chosen key is pressed. If so, the mouse is made invisible and
    the view is panned based on the relative position between the mouse's
    position on the key press and its current position.

    @param initialPos The position of the mouse when the chosen key is pressed.
    @param originalView The central position of the view when the chosen key
    is pressed.
    @param worldSizeX The x-direction size of the simulation world.
    @param worldSizeY The y-direction size of the simulation world.
    @param keyBool The boolean value of the chosen key, isKeyPressed.

    @return Void.
*/
void SurvivalScene::checkForViewPan(sf::Vector2i initialPos)
{
    window.setMouseCursorVisible(false);

    sf::Vector2f effectiveZoom = getEffectiveZoom(wSize.x, wSize.y);
    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
    sf::Vector2f relPos = static_cast<sf::Vector2f>(initialPos - viewPos);

    if(effectiveZoom.y > effectiveZoom.x)
        relPos *= effectiveZoom.y;
    else
        relPos *= effectiveZoom.x;


    worldView.move(relPos);// + originalView);

    window.setView(worldView);
}




/**
    Adjusts the camera to center on a chosen ball within the simulation if a
    chosen key is held.

    @param ballIndex The ball to focus on, specified by an integer index.
    @param keyBool The boolean isKeyPressed value of the chosen key.

    @return Void.
*/
void SurvivalScene::focusOnBall(int ballIndex)
{
    sf::Vector2f relPos = ballSim.getBallPosition(ballIndex);
    if(!std::isnan(relPos.x) && !std::isnan(relPos.y))
    {
        worldView.setCenter(relPos);
        window.setView(worldView);
    }
}

void SurvivalScene::spawnFromJson(sf::Vector2f position, sf::Vector2f velocity)
{
    using json = nlohmann::json;
    std::ifstream initInput("./json/spawnparams.json");
    if(json::accept(initInput))
    {
        std::ifstream input("./json/spawnparams.json");
        json j;
        input >> j;

        for(json &currJ : j["Ball"])
        {
            BallSpawnVals sVals;
            if(beParser::checkBallJson(currJ, sVals))
                ballSim.spawnNewBall(sVals.position+position,
                                     sVals.velocity+velocity,
                                     sVals.radius,
                                     sVals.mass);
        }
        for(json &currJ : j["Ballgrid"])
        {
            BallGridSpawnVals sVals;
            if(beParser::checkBallGridJson(currJ, sVals))
                ballSim.createBallGrid(sVals.dimensions.x,
                                       sVals.dimensions.y,
                                       sVals.spacing,
                                       sVals.position+position,
                                       sVals.velocity+velocity,
                                       sVals.mass,
                                       sVals.radius);
        }
        for(json &currJ : j["AABB"])
        {
            AABBSpawnVals sVals;
            if(beParser::checkAABBJson(currJ, sVals))
                ballSim.spawnNewRect(sVals.position + position,
                                     sVals.dimensions.x,
                                     sVals.dimensions.y);
        }
    }
}

/**
    Scales the view to the window based on the current window and world sizes.
    This function ensure the world level zoom is preserved as well as accounting
    for vertical aspect ratios by checking for effective zooms in each direction.

    @param sizeX The x-direction size of the window.
    @param sizeY The y-direction size of the window.
    @param worldSizeX The x-direction size of the simulation world.
    @param worldSizeY The y-direction size of the simulation world.

    @return Void.
*/
void SurvivalScene::adjustViewSize(sf::Vector2u newSize)
{
    sf::Vector2f effectiveZoom = getEffectiveZoom(wSize.x, wSize.y);
    worldView.setSize(newSize.x, newSize.y);
    GUIView.setSize(newSize.x, newSize.y);

    if(effectiveZoom.y > effectiveZoom.x)
        worldView.zoom(effectiveZoom.y);
    else
        worldView.zoom(effectiveZoom.x);

    window.setView(worldView);
}

/**
    Resets the window view to a zoom of 1, centered on the simulation world
    centre.

    @return Void.
*/
void SurvivalScene::resetView()
{
    currentZoom = 1.0;
    sf::Vector2i woSize = ballSim.getWorldSize();
    worldView.setCenter(woSize.x/2,woSize.y/2);
    adjustViewSize({window.getSize().x, window.getSize().y});//, currentZoom);

    window.setView(worldView);
}


/**
    Changes the drawn rectangle which is the boundary of the simulation.

    @param worldSize The size of the simulation world.

    @return Void.
*/
void SurvivalScene::changeBoundaryRect(sf::Vector2i worldSize)
{
    sf::Color grey(50,50,50);
    boundaryRect.setSize(static_cast<sf::Vector2f>(worldSize));
    boundaryRect.setOutlineColor(grey);
    boundaryRect.setOutlineThickness(5);
    boundaryRect.setFillColor(sf::Color::Transparent);
    boundaryRect.setPosition({0,0});
}

/**
    Zooms the world based on direction of mouse wheel scroll if a chosen key is
    pressed.

    @param keyPress The isKeyPressed boolean value for a chosen key.
    @param delta The mousewheel delta value. Positive if the mousewheel is
    scrolled up, negative otherwise.

    @return Void.
*/
void SurvivalScene::mouseWheelZoom(bool keyPress, float delta)
{
    if(keyPress)
    {
        //std::cout << delta << "\n";
        if(delta > 0)
            zoomToMouse(0.97);
        else
            zoomToMouse(1.03);
    }
    canZoom = false;
}

/**
    Clicks on a User Interface window.

    @return Void.
*/
void SurvivalScene::clickOnUI()
{
    container.checkMouseIntersection(window, GUIView, worldView);
    mouseOnUIWhenClicked = container.doesMIntExist();
    container.clickOnUI(window);

}

/**
    Sets the current User Interface window to not be draggable.

    @return Void.
*/
void SurvivalScene::resetUIClick()
{
    container.resetUIClick();
    clickedWindowToDrag = false;
}


/**
    Function which handles all events to do with checking for UI interaction
    or adjusting the view.

    @param &event The event case to process.

    @return Void.
*/
void SurvivalScene::mouseViewEvents(sf::Event &event)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        clickOnUI();
    }

    else if(event.type == sf::Event::MouseMoved)
    {
        container.checkMouseIntersection(window, GUIView, worldView);
        container.doesMIntExist();
    }

    else if(event.type == sf::Event::MouseWheelScrolled)
    {
        float delta = event.mouseWheelScroll.delta;
        mouseWheelZoom(canZoom, delta);
    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        resetUIClick();
    }

}


/**
    Function which handles all events to do with generating interactions with
    the User Interface.

    @param &event The event case to process.

    @return Void.
*/
void SurvivalScene::mouseUIEvents(sf::Event &event)
{
        if(event.type == sf::Event::MouseButtonPressed)
            clickedWindowToDrag = container.isWindowDraggable();
}




/**
    Function which handles events on window resize.

    @param &event The event case to process.

    @return Void.
*/
void SurvivalScene::resizeEvents(sf::Event &event)
{
    if(event.type == sf::Event::Resized)
        adjustViewSize({event.size.width, event.size.height});//, currentZoom);
}





void SurvivalScene::setSpawnValues(float value,
                                        SpawnQuantity toChange)
{
    //float density = 1.0f;
    switch(toChange)
    {
        case(SQ_MASS):
            spawnMass = value;
            //spawnRadius = density*value;
            break;
        case(SQ_RADIUS):
            spawnRadius = value;
            //spawnMass = density*value;
            break;
        case(SQ_DENSITY):
            break;
        default:
            break;
    }
}

void SurvivalScene::events(sf::Event &event)
{
    mouseViewEvents(event);

    if(!mouseOnUIWhenClicked.first) {}
    else
        mouseUIEvents(event);

    keyEvents(event);
    resizeEvents(event);
}

void SurvivalScene::togglePause()
{
    requestScene(SceneEnum::SCENE_PAUSEMENU);
}

void SurvivalScene::load()
{
    if(!isLoaded)
    {
        isLoaded = true;
        wSize = ballSim.getWorldSize();
        changeBoundaryRect(wSize);
        resetView();
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
            {"rstView",     [&]{resetView();}},
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

        loadUI("./json/SurvivalSceneUI.json", container);
        loadKeybinds("./json/keybinds.json", "SurvivalScene");

    }
}

void SurvivalScene::unload()
{
    isLoaded = false;
    container.destroyAllWindows();
    ballSim.clearSimulation();

}

void SurvivalScene::redraw(sf::RenderWindow &_window)
{
    ballSim.drawSampledPositions(_window);
    ballSim.drawBalls(_window);
    window.draw(boundaryRect);

    container.renderWindows(_window, GUIView, worldView);
}

void SurvivalScene::update(sf::RenderWindow &_window)
{
    if(!mouseOnUIWhenClicked.first)
    {
    }
    if(clickedWindowToDrag)
        container.dragWindow(_window);

    window.setMouseCursorVisible(true);

    exePressedKeys();

    mousePosOnPan = sf::Mouse::getPosition(window);

    ballSim.universeLoop(currentFrameTime, timestep);

    timeToNextSpawn -= currentFrameTime;
}

SurvivalScene::SurvivalScene(sf::RenderWindow &_window, sf::Time &_targetFTime,
                     sf::Time &_currentFTime, float &_currentFPS) : window{_window},
                     currentFrameTime{_currentFTime}, currentFPS{_currentFPS},
                     timestep{_targetFTime}
{

}


