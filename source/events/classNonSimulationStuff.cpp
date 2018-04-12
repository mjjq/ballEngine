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

#include "../../headers/classNonSim.h"
#include "../../headers/classTextElementBase.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"

/**
    Increases the spawn mass by 1.
*/
void NonSimulationStuff::increaseMass()
{
    spawnMass += 1;
    //std::cout << "Hello\n";
}


/**
    If the mouse button is pressed, draws a line between position where
    mouse was clicked and the current position of the mouse.

    @param &initPos The click position of the mouse.
    @param mouseType Boolean which draws line if set to true.
*/
void NonSimulationStuff::checkMBPress(sf::Vector2i &initPos, bool mouseType)
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
sf::Vector2f NonSimulationStuff::velocityFromMouse(sf::Vector2i mousePosOnClick,
                                                            int spawnVelFactor)
{

    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
    sf::Vector2i mousePosOnRelease = static_cast<sf::Vector2i>
                                     (window.mapPixelToCoords(viewPos));

    sf::Vector2i scaledVector = (spawnVelFactor)*(mousePosOnRelease-
                                                  mousePosOnClick);

    float windowDiagSize = pow(windowSizeX*windowSizeX +
                               windowSizeY*windowSizeY, 0.5);

    sf::Vector2f velocity = static_cast<sf::Vector2f>(scaledVector)/windowDiagSize;
    timeToNextSpawn = minTimeToNextSpawn;

    return velocity;
}

/**
    Applies a zoom to the rendered view, and centres the view to the position
    of the mouse. The if statement imposes a maximum zoom out limit.

    @param zoomFactor The factor by which to zoom the current view.
*/
void NonSimulationStuff::zoomToMouse(float zoomFactor)
{
    if(currentZoom*zoomFactor < 2.0)
    {
        currentZoom *= zoomFactor;
        worldView.zoom(zoomFactor);
        sf::Vector2i viewPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(viewPos);
        worldView.setCenter(mousePos);
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
sf::Vector2f NonSimulationStuff::getEffectiveZoom(int worldSizeX, int worldSizeY)
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
void NonSimulationStuff::checkForViewPan(sf::Vector2i initialPos,
    sf::Vector2f originalView, int worldSizeX, int worldSizeY, bool keyBool)
{
    if(keyBool==true)
    {
        window.setMouseCursorVisible(false);

        sf::Vector2f effectiveZoom = getEffectiveZoom(worldSizeX, worldSizeY);
        sf::Vector2i viewPos = sf::Mouse::getPosition(window);
        sf::Vector2f relPos = static_cast<sf::Vector2f>(initialPos - viewPos);

        if(effectiveZoom.y > effectiveZoom.x)
            relPos *= effectiveZoom.y;
        else
            relPos *= effectiveZoom.x;


        worldView.setCenter(relPos + originalView);

        window.setView(worldView);
    }
    else
        window.setMouseCursorVisible(true);
}


/**
    Adjusts the camera to center on a chosen ball within the simulation if a
    chosen key is held.

    @param ballIndex The ball to focus on, specified by an integer index.
    @param keyBool The boolean isKeyPressed value of the chosen key.

    @return Void.
*/
void NonSimulationStuff::focusOnBall(int ballIndex, bool keyBool)
{
    if(keyBool==true)
    {
        sf::Vector2f relPos = ballSim.getBallPosition(ballIndex);

        worldView.setCenter(relPos);
        window.setView(worldView);
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
void NonSimulationStuff::adjustViewSize(int sizeX, int sizeY, int worldSizeX, int worldSizeY)
{
    sf::Vector2f effectiveZoom = getEffectiveZoom(worldSizeX,worldSizeY);
    worldView.setSize(sizeX,sizeY);
    GUIView.setSize(sizeX, sizeY);
    windowSizeX = sizeX;
    windowSizeY = sizeY;

    if(effectiveZoom.y > effectiveZoom.x)
        worldView.zoom(effectiveZoom.y);
    else
        worldView.zoom(effectiveZoom.x);

    window.setView(worldView);
}

/**
    Toggles the window between windowed and fullscreen mode.

    @return Void.
*/
void NonSimulationStuff::toggleFullScreen()
{
    if(!isFullScreen)
    {
        prevWindowSizeX = windowSizeX;
        prevWindowSizeY = windowSizeY;
        prevWindowPosition = window.getPosition();

        sf::VideoMode mode = sf::VideoMode::getDesktopMode();
        window.setSize({mode.width,mode.height});
        window.create(sf::VideoMode(mode.width,mode.height), "ballEngine", sf::Style::None);
        adjustViewSize(mode.width, mode.height, wSize.x, wSize.y);
        isFullScreen = true;
    }
    else
    {
        window.setSize({prevWindowSizeX,prevWindowSizeY});
        window.create(sf::VideoMode(prevWindowSizeX,prevWindowSizeY), "ballEngine");
        adjustViewSize(prevWindowSizeX,prevWindowSizeY, wSize.x, wSize.y);
        window.setPosition(prevWindowPosition);
        isFullScreen = false;
    }
}

/**
    Resets the window view to a zoom of 1, centered on the simulation world
    centre.

    @return Void.
*/
void NonSimulationStuff::resetView()
{
    currentZoom = 1.0;
    sf::Vector2i woSize = ballSim.getWorldSize();
    worldView.setCenter(woSize.x/2,woSize.y/2);
    adjustViewSize(window.getSize().x, window.getSize().y, woSize.x, woSize.y);//, currentZoom);

    window.setView(worldView);
}


/**
    Changes the drawn rectangle which is the boundary of the simulation.

    @param worldSize The size of the simulation world.

    @return Void.
*/
void NonSimulationStuff::changeBoundaryRect(sf::Vector2i worldSize)
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
void NonSimulationStuff::mouseWheelZoom(bool keyPress, float delta)
{
    if(keyPress)
    {
        //std::cout << delta << "\n";
        if(delta > 0)
            zoomToMouse(0.97);
        else
            zoomToMouse(1.03);
    }
}

/**
    Clicks on a User Interface window.

    @return Void.
*/
void NonSimulationStuff::clickOnUI()
{
    container.checkMouseIntersection(window, GUIView, worldView);
    mouseOnUIWhenClicked = container.doesMIntExist();
    container.clickOnUI(window);

}

/**
    Sets the current User Interface window to not be draggable.

    @return Void.
*/
void NonSimulationStuff::resetUIClick()
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
void NonSimulationStuff::mouseViewEvents(sf::Event &event)
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
        mouseWheelZoom(sf::Keyboard::isKeyPressed(sf::Keyboard::Space), delta);
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
void NonSimulationStuff::mouseUIEvents(sf::Event &event)
{
        if(event.type == sf::Event::MouseButtonPressed)
            clickedWindowToDrag = container.isWindowDraggable();
}


/**
    Function which handles all events to do with generating interactions with
    the simulation world.

    @param &event The event case to process.

    @return Void.
*/
void NonSimulationStuff::mouseWorldEvents(sf::Event &event)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i viewPos = sf::Mouse::getPosition(window);
        mousePosOnClick = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));
    }

    else if(event.type == sf::Event::EventType::MouseButtonReleased
                            && event.mouseButton.button==sf::Mouse::Left
                            && !(timeToNextSpawn > sf::milliseconds(0)))
    {
        sf::Vector2f velocity = velocityFromMouse(mousePosOnClick, spawnVelFactor);
        ballSim.spawnNewBall(static_cast<sf::Vector2f>(mousePosOnClick),velocity,spawnRadius,spawnMass);
    }

    /*if(event.type == sf::Event::EventType::MouseButtonReleased
                    && event.mouseButton.button==sf::Mouse::Right
                    && !(timeToNextSpawn > sf::milliseconds(0)))
    {
        sf::Vector2f velocity = velocityFromMouse(mousePosOnClick);
        spawnNewBall(static_cast<sf::Vector2f>(mousePosOnClick),velocity,spawnRadius,-spawnMass);
    }*/

    else if(event.type == sf::Event::EventType::MouseButtonReleased
                    && event.mouseButton.button==sf::Mouse::Middle
                    && !(timeToNextSpawn > sf::milliseconds(0)))
    {
        sf::Vector2f velocity = velocityFromMouse(mousePosOnClick, spawnVelFactor);
        ballSim.createBallGrid(ballGridWidth,ballGridHeight,ballGridSpacing,
                        static_cast<sf::Vector2f>(mousePosOnClick),velocity,spawnMass,spawnRadius);
    }
}

/**
    Function which handles general key based events under some combination of
    primary key presses e.g. ctrl, alt, shift.

    @param &newLayerKeys Vector of isKeyPressed boolean.
    @param &event The event case to process.

    @return Void.
*/
void NonSimulationStuff::newLayerEvent(std::vector<bool> &newLayerKeys, sf::Event &event)
{
    if(newLayerKeys[0]&&(!newLayerKeys[1]))
    {
        if(event.key.code == sf::Keyboard::C)
            ballSim.toggleCollisions();
        else if(event.key.code == sf::Keyboard::F)
            ballSim.toggleForces();
        else if(event.key.code == sf::Keyboard::K)
            ballSim.changeBallColour();
        else if(event.key.code == sf::Keyboard::R)
            spawnRadius -= 1;
        else if(event.key.code == sf::Keyboard::M)
            spawnMass -= 1;
    }
    else if(newLayerKeys[2])
    {
        if(event.key.code == sf::Keyboard::Return)
            toggleFullScreen();
    }
}


/**
    Function which handles general key based events. Events are processed provided
    there are no key primary keys held. If these keys are held, the newLayerEvent
    events are processed instead.

    @param &event The event case to process.

    @return Void.
*/
void NonSimulationStuff::keyEvents(sf::Event &event)
{
    std::vector<bool> newLayerKeys = {sf::Keyboard::isKeyPressed(sf::Keyboard::LControl),
                                       sf::Keyboard::isKeyPressed(sf::Keyboard::LShift),
                                       sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)};
    bool newLayerPressed = false;
    if(std::find(newLayerKeys.begin(), newLayerKeys.end(), true) != newLayerKeys.end())
        newLayerPressed = true;

    if(event.type == sf::Event::EventType::KeyPressed)
    {
        if(!newLayerPressed)
        {
            if(event.key.code == sf::Keyboard::PageUp)
                incTimeStep(sf::milliseconds(1.0/60.0));
            else if(event.key.code == sf::Keyboard::PageDown && timestep>sf::milliseconds(15))
                decTimeStep(sf::milliseconds(1.0/60.0));
            else if(event.key.code == sf::Keyboard::Comma)
                ballSim.decSimStep(1.0);
            else if(event.key.code == sf::Keyboard::Period)
                ballSim.incSimStep(1.0);
            else if(event.key.code == sf::Keyboard::Delete)
                ballSim.clearSimulation();
            else if(event.key.code == sf::Keyboard::Dash)
                zoomToMouse(2.0f);
            else if(event.key.code == sf::Keyboard::Equal)
                zoomToMouse(0.5f);
            else if(event.key.code == sf::Keyboard::Num0)
                resetView();
            else if(event.key.code == sf::Keyboard::P)
                ballSim.toggleSimPause();
            else if(event.key.code == sf::Keyboard::Space)
            {
                recentViewCoords = worldView.getCenter();
                mousePosOnPan = sf::Mouse::getPosition(window);
            }
            else if(event.key.code == sf::Keyboard::M)
                spawnMass += 1;
            else if(event.key.code == sf::Keyboard::R)
                spawnRadius += 1;
        }

        newLayerEvent(newLayerKeys, event);
    }
}


/**
    Function which handles player key held cases.

    @param player The ball index which the player controls.

    @return Void.
*/
void NonSimulationStuff::playerKeysDown(int player)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F))
        focusOnBall(0, sf::Keyboard::isKeyPressed(sf::Keyboard::F));

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        ballSim.pushBall(0.01, 0, player);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        ballSim.pushBall(0.01, 180, player);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        ballSim.pushBall(0.01, 270, player);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        ballSim.pushBall(0.01, 90, player);
}


/**
    Function which handles events on window resize.

    @param &event The event case to process.

    @return Void.
*/
void NonSimulationStuff::resizeEvents(sf::Event &event)
{
    if(event.type == sf::Event::Resized)
        adjustViewSize(event.size.width, event.size.height, wSize.x, wSize.y);//, currentZoom);
}


/**
    Increases the rendering timestep.

    @param delta The amount to increase the rendering timestep by.

    @return Void.
*/
void NonSimulationStuff::incTimeStep(sf::Time delta)
{
    if(delta>sf::milliseconds(0))
        timestep+=delta;
}


/**
    Decreases the rendering timestep.

    @param delta The amount to decrease the rendering timestep by.

    @return Void.
*/
void NonSimulationStuff::decTimeStep(sf::Time delta)
{
    if(delta>sf::milliseconds(0) && timestep>delta)
        timestep-=delta;
}

/**
    Returns the current x-direction window size.

    @return x-direction window size.
*/
float NonSimulationStuff::getWindowSizeX()
{
    return windowSizeX;
}

void NonSimulationStuff::mainLoop()
{
    while(window.isOpen())
    {
        window.clear();
        //std::pair<bool,int> mouseOnUI = container.doesMIntExist();
        sf::Event event;
        while(window.pollEvent(event))
        {
            window.setKeyRepeatEnabled(false);

            if(event.type == sf::Event::Closed)
                window.close();

            mouseViewEvents(event);

            if(!mouseOnUIWhenClicked.first)
                mouseWorldEvents(event);
            else
                mouseUIEvents(event);

            keyEvents(event);
            resizeEvents(event);
        }

        if(!mouseOnUIWhenClicked.first)
        {
            checkMBPress(mousePosOnClick,sf::Mouse::isButtonPressed(sf::Mouse::Left));
            checkMBPress(mousePosOnClick,sf::Mouse::isButtonPressed(sf::Mouse::Middle));
            checkMBPress(mousePosOnClick,sf::Mouse::isButtonPressed(sf::Mouse::Right));
        }
        if(clickedWindowToDrag)
            container.dragWindow(window);

        checkForViewPan(mousePosOnPan,recentViewCoords, wSize.x, wSize.y, sf::Keyboard::isKeyPressed(sf::Keyboard::Space));
        playerKeysDown(0);

        ballSim.universeLoop();

        ballSim.drawBalls(window);
        window.draw(boundaryRect);

        container.renderWindows(window, GUIView, worldView);

        window.display();

        sf::sleep(timestep);
        timeToNextSpawn -= timestep;
    }
}

NonSimulationStuff::NonSimulationStuff(int m_windowSizeX, int m_windowSizeY, int m_spawnVelFactor,
                 float m_spawnRadius, float m_spawnMass, float m_ballGridSpacing, int m_ballGridHeight,
                                    int m_ballGridWidth, bool m_simFitsInWindow, BallUniverse m_sim) :

windowSizeX{m_windowSizeX}, windowSizeY{m_windowSizeY}, spawnVelFactor{m_spawnVelFactor},
            spawnRadius{m_spawnRadius}, spawnMass{m_spawnMass}, ballGridSpacing{m_ballGridSpacing},
            ballGridHeight{m_ballGridHeight}, ballGridWidth{m_ballGridWidth}, simFitsInWindow{m_simFitsInWindow}, ballSim{m_sim}
{
    wSize = ballSim.getWorldSize();
    changeBoundaryRect(wSize);
    resetView();

    container.addWindow({0,100}, 250, 250, true, false);
    container.addWindow({0,0}, 250, 50, true, false);
    container.addWindow({0,400}, 250, 50, true, false);
    container.addWindow({0,500}, 250, 150, true, false);

    container.getWindow(0).addElement("./fonts/cour.ttf", "No. Balls:", 16, {0,0}, &ballSim.getNumOfBalls());
    container.getWindow(0).addElement("./fonts/cour.ttf", "Spawn Mass:", 16, {00,30}, &spawnMass);
    container.getWindow(0).addElement("./fonts/cour.ttf", "Spawn Radius:", 16, {00,50}, &spawnRadius);
    container.getWindow(0).addElement("./fonts/cour.ttf", "Forces Enabled:", 16, {0,70}, &ballSim.getForcesEnabled());
    container.getWindow(0).addElement("./fonts/cour.ttf", "Collisions Enabled:", 16, {0,90}, &ballSim.getCollisionsEnabled());
    container.getWindow(0).addElement("./fonts/cour.ttf", "Timestep:", 16, {0,110}, &ballSim.getTimeStep());

    container.getWindow(0).addButton("./fonts/cour.ttf", "Mass +", 12, {10,180}, {60,30}, [&]{spawnMass+=1;});
    container.getWindow(0).addButton("./fonts/cour.ttf", "Mass -", 12, {90,180}, {60,30}, [&]{if(spawnMass>1){spawnMass-=1;}});
    container.getWindow(0).addButton("./fonts/cour.ttf", "Rst Mass", 12, {170,180}, {60,30}, [&]{spawnMass=1;});
    container.getWindow(0).addButton("./fonts/cour.ttf", "Rad +", 12, {10,220}, {60,30}, [&]{spawnRadius+=1;});
    container.getWindow(0).addButton("./fonts/cour.ttf", "Rad -", 12, {90,220}, {60,30}, [&]{if(spawnRadius>1){spawnRadius-=1;}});
    container.getWindow(0).addButton("./fonts/cour.ttf", "Rst Rad", 12, {170,220}, {60,30}, [&]{spawnRadius=10;});

    /*std::string font, std::string text, int fontSize, sf::Vector2f position, sf::Vector2f bSize,
                                                bool fixedToWin, std::function<void> *func, sf::Color color*/

    container.getWindow(1).addElement("./fonts/cour.ttf", "WindowSizeX:", 16, {00,00}, &windowSizeX);
    container.getWindow(1).addElement("./fonts/cour.ttf", "WindowSizeY:", 16, {0,20}, &windowSizeY);
    //container.getWindow(1).addButton("./fonts/cour.ttf", "Rad -", 16, {0,50}, {80,40}, increaseMass);

    container.getWindow(2).addButton("./fonts/cour.ttf", "Star", 12, {10,10}, {60,30}, [&]{spawnRadius=50;spawnMass=100;});
    container.getWindow(2).addButton("./fonts/cour.ttf", "Planet", 12, {90,10}, {60,30}, [&]{spawnRadius=10;spawnMass=1;});
    container.getWindow(2).addButton("./fonts/cour.ttf", "Asteroid", 12, {170,10}, {60,30}, [&]{spawnRadius=3;spawnMass=0.01;});

    container.getWindow(3).addElement("./fonts/cour.ttf", "Total KE: ", 16, {0,0}, &ballSim.getTotalKE());
    //container.getWindow(3).addElement("./fonts/cour.ttf", "Total Momentum: ", 16, {0,20}, &ballSim.getTotalMomentum());
    container.getWindow(3).addElement("./fonts/cour.ttf", "Total Energy: ", 16, {0,20}, &ballSim.getTotalEnergy());
    container.getWindow(3).addElement("./fonts/cour.ttf", "Use RK4: ", 16, {0,40}, &ballSim.getUseRK4());
    container.getWindow(3).addButton("./fonts/cour.ttf", "Trj", 12, {10,80}, {60,30}, [&]{ballSim.toggleTrajectories();});
    container.getWindow(3).addButton("./fonts/cour.ttf", "Toggle\nRK4", 12, {90,80}, {60,30}, [&]{ballSim.toggleRK4();});



}
