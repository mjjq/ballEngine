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

#include "classGameScene.h"
#include "sfVectorMath.h"
#include "stringConversion.h"
#include "jsonParsing.h"



/**
    If the mouse button is pressed, draws a line between position where
    mouse was clicked and the current position of the mouse.

    @param &initPos The click position of the mouse.
    @param mouseType Boolean which draws line if set to true.
*/
void GameScene::checkMBPress(sf::Vector2i &initPos, bool mouseType)
{
    sf::Vector2i finalPos;
    sf::Vertex line[2];

    if((mouseType == true))
    {
        sf::Vector2i viewPos = sf::Mouse::getPosition(window);
        finalPos = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));

        line[0] = sf::Vertex(static_cast<sf::Vector2f>(initPos),
                                              sf::Color::White);
        line[1] = sf::Vertex(static_cast<sf::Vector2f>(finalPos),
                                              sf::Color::White);
        window.draw(line, 2, sf::Lines);
    }
}

/**
    Generates a spawn velocity for a new ball proportional to the distance
    between the mouse position on click and the current mouse position.

    @param mousePosOnClick The mouse position when a chosen mouse button was
    clicked.
    @param spawnVelFactor The factor by which the aforementioned distance is
    multiplied. A larger value will generate a larger spawn velocity.

    @return The calculated velocity.
*/
sf::Vector2f GameScene::velocityFromMouse(sf::Vector2i _mousePosOnClick,
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
void GameScene::zoomToMouse(float zoomFactor)
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
sf::Vector2f GameScene::getEffectiveZoom(int worldSizeX, int worldSizeY)
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
void GameScene::checkForViewPan(sf::Vector2i initialPos)
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
void GameScene::focusOnBall(int ballIndex)
{
    sf::Vector2f relPos = ballSim.getObjPosition(ballIndex);
    if(!std::isnan(relPos.x) && !std::isnan(relPos.y))
    {
        worldView.setCenter(relPos);
        window.setView(worldView);
    }
}

void GameScene::spawnFromJson(sf::Vector2f position, sf::Vector2f velocity)
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
                ballSim.spawnNewBall({sVals.position+position,
                                     sVals.velocity+velocity,
                                     {sVals.radius, 0.0f},
                                     sVals.mass,
                                     0.0f, 0.0f});
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
        /*for(json &currJ : j["statAABB"])
        {
            AABBSpawnVals sVals;
            if(beParser::checkAABBJson(currJ, sVals))
                ballSim.spawnStaticRect(sVals.position + position,
                                     sVals.dimensions.x,
                                     sVals.dimensions.y, 0.0f);
        }
        for(json &currJ : j["dynAABB"])
        {
            AABBSpawnVals sVals;
            (if(beParser::checkAABBJson(currJ, sVals))
                ballSim.spawnNewRect(sVals.position + position,
                                     sVals.dimensions.x,
                                     sVals.dimensions.y,
                                     {velocity},
                                     5.0f, 0.0f);
        }*/
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
void GameScene::adjustViewSize(sf::Vector2u newSize)
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
void GameScene::resetCamera()
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
void GameScene::changeBoundaryRect(sf::Vector2i worldSize)
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
void GameScene::mouseWheelZoom(bool keyPress, float delta)
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
    Function which handles all events to do with checking for UI interaction
    or adjusting the view.

    @param &event The event case to process.

    @return Void.
*/
void GameScene::mouseViewEvents(sf::Event &event)
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
    the simulation world.

    @param &event The event case to process.

    @return Void.
*/
void GameScene::mouseWorldEvents(sf::Event &event)
{

}





void GameScene::setSpawnValues(float value,
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
        case(SQ_ROTATION):
            spawnRotation = value;
            break;
        case(SQ_ROTRATE):
            spawnRotRate = value;
            break;
        case(SQ_CREST):
            spawnCoefRest = value;
            break;
        case(SQ_CFRICTION) :
            spawnCoefFriction = value;
            break;
        case(SQ_DENSITY):
            break;
        default:
            break;
    }
}

void GameScene::events(sf::Event &event)
{
    mouseViewEvents(event);

    if(!mouseOnUIWhenClicked.first)
        mouseWorldEvents(event);
    else
        mouseUIEvents(event);

    KeyBinds::keyEvents(event, pressedKeyStack, releasedKeyStack);
    resizeEvents(event);
}

void GameScene::togglePause()
{
    requestScene(SceneEnum::SCENE_PAUSEMENU);
}

void GameScene::load()
{

}

void GameScene::unload()
{
    isLoaded = false;
    container.destroyAllWindows();
    ballSim.clearSimulation();

}

void GameScene::redraw(sf::RenderWindow &_window)
{
    //ballSim.drawSampledPositions(_window);
    //ballSim.drawBalls(_window);
    window.draw(boundaryRect);

    container.renderWindows(_window, GUIView, worldView);
}

void GameScene::update(sf::RenderWindow &_window)
{

}

GameScene::GameScene(sf::RenderWindow &_window, sf::Time &_targetFTime,
                     sf::Time &_currentFTime, float &_currentFPS) :
                    Scene(_window), targetFrameTime{_targetFTime},
                    currentFrameTime{_currentFTime}, currentFPS{_currentFPS}
{

}


