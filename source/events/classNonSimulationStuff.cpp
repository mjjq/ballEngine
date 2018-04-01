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


void NonSimulationStuff::increaseMass()
{
    spawnMass += 1;
    std::cout << "Hello\n";
}

void NonSimulationStuff::checkMBPress(sf::Vector2i &initPos, bool mouseType)
{
    sf::Vector2i finalPos;
    sf::Vertex line[2];

    if((mouseType == true))
    {
        sf::Vector2i viewPos = sf::Mouse::getPosition(window);
        finalPos = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));
        //finalPos = sf::Mouse::getPosition(window);
        line[0] = sf::Vertex(static_cast<sf::Vector2f>(initPos),sf::Color::White);
        line[1] = sf::Vertex(static_cast<sf::Vector2f>(finalPos),sf::Color::White);
        window.draw(line, 2, sf::Lines);
    }
}

sf::Vector2f NonSimulationStuff::velocityFromMouse(sf::Vector2i mousePosOnClick, int spawnVelFactor)
{

    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
    sf::Vector2i mousePosOnRelease = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));
    //sf::Vector2i mousePosOnRelease = sf::Mouse::getPosition(window);
    sf::Vector2i scaledVector = (spawnVelFactor)*(mousePosOnRelease-mousePosOnClick);
    float windowDiagSize = pow(windowSizeX*windowSizeX + windowSizeY*windowSizeY, 0.5);
    sf::Vector2f velocity = static_cast<sf::Vector2f>(scaledVector)/windowDiagSize;
    timeToNextSpawn = minTimeToNextSpawn;

    return velocity;
}

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

sf::Vector2f NonSimulationStuff::getEffectiveZoom(int worldSizeX, int worldSizeY)
{
    sf::Vector2f effectiveZoom;
    if(simFitsInWindow)
    {
        float zoom = currentZoom;
        float winSizeX = window.getSize().x;
        float winSizeY = window.getSize().y;
        sf::Vector2i viewPos = sf::Mouse::getPosition(window);
        float xFactor = zoom*static_cast<float>(worldSizeX)/static_cast<float>(winSizeX);
        float yFactor = zoom*static_cast<float>(worldSizeY)/static_cast<float>(winSizeY);
        effectiveZoom = {xFactor,yFactor};
    }
    else
        effectiveZoom = {1,1};
    return effectiveZoom;
}

void NonSimulationStuff::checkForViewPan(sf::Vector2i initialPos, sf::Vector2f originalView, int worldSizeX, int worldSizeY, bool keyBool)
{
    if(keyBool==true)
    {
        window.setMouseCursorVisible(false);
        sf::Vector2f effectiveZoom = getEffectiveZoom(worldSizeX, worldSizeY);
        sf::Vector2i viewPos = sf::Mouse::getPosition(window);
        sf::Vector2f relPos = static_cast<sf::Vector2f>(initialPos - viewPos);
        //sfVectorMath::printVector(viewPos);
        //std::cout <<"\n";
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

void NonSimulationStuff::adjustViewSize(int sizeX, int sizeY, int worldSizeX, int worldSizeY)//, float zoom)
{
    sf::Vector2f effectiveZoom = getEffectiveZoom(worldSizeX,worldSizeY);
    worldView.setSize(sizeX,sizeY);
    GUIView.setSize(sizeX, sizeY);
    windowSizeX = sizeX;
    windowSizeY = sizeY;

    if(effectiveZoom.y > effectiveZoom.x)
    {
        worldView.zoom(effectiveZoom.y);
        //GUIView.zoom(effectiveZoom.y);
    }
    else
    {
        worldView.zoom(effectiveZoom.x);
        //GUIView.zoom(effectiveZoom.x);
    }

    window.setView(worldView);
}

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
        adjustViewSize(mode.width, mode.height, wSize.x, wSize.y);//, zoom);
        isFullScreen = true;
    }
    else
    {
        window.setSize({prevWindowSizeX,prevWindowSizeY});
        window.create(sf::VideoMode(prevWindowSizeX,prevWindowSizeY), "ballEngine");
        adjustViewSize(prevWindowSizeX,prevWindowSizeY, wSize.x, wSize.y);//, zoom);
        window.setPosition(prevWindowPosition);
        isFullScreen = false;
    }
}

void NonSimulationStuff::resetView()
{
    currentZoom = 1.0;
    sf::Vector2i woSize = ballSim.getWorldSize();
    worldView.setCenter(woSize.x/2,woSize.y/2);
    adjustViewSize(window.getSize().x, window.getSize().y, woSize.x, woSize.y);//, currentZoom);

    window.setView(worldView);
}

/*void NonSimulationStuff::drawBalls()
{
    std::vector<Ball>* arrayAddress = ballSim.getBallArrayAddress();
    for(int i=0; i<arrayAddress->size(); i++)
        window.draw(arrayAddress->at(i));
}*/

void NonSimulationStuff::changeBoundaryRect(sf::Vector2i worldSize)
{
    sf::Color grey(50,50,50);
    boundaryRect.setSize(static_cast<sf::Vector2f>(worldSize));
    boundaryRect.setOutlineColor(grey);
    boundaryRect.setOutlineThickness(5);
    boundaryRect.setFillColor(sf::Color::Transparent);
    boundaryRect.setPosition({0,0});
}

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

void NonSimulationStuff::clickOnUI()
{
    container.checkMouseIntersection(window, GUIView, worldView);
    mouseOnUIWhenClicked = container.doesMIntExist();
    container.clickOnUI(window);

}

void NonSimulationStuff::resetUIClick()
{
    container.resetUIClick();
    clickedWindowToDrag = false;
    //container.checkMouseIntersection(window, GUIView, worldView);
    //container.doesMIntExist();
}

void NonSimulationStuff::resetButtonPress()
{
    /*bool windowBool = std::get<0>(mouseOnUIWhenClicked);
    int windowIndex = std::get<1>(mouseOnUIWhenClicked);
    if(windowIndex != -1)
    {
        std::cout << windowIndex << "\n";
        container.getWindow(windowIndex).resetButtonPair();
    }*/
    //container.resetUIClick();
    //mouseOnButtonWhenClicked = std::make_pair(false, -1);
}


void NonSimulationStuff::mouseViewEvents(sf::Event &event)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        clickOnUI();
    }

    else if(event.type == sf::Event::MouseMoved)
    {
        //resetButtonPress();
        //if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        //std::cout << mouseOnUIWhenClicked.first << "\n";
        //mouseOnUIWhenClicked = container.doesMIntExist();
        container.checkMouseIntersection(window, GUIView, worldView);
        container.doesMIntExist();
        //mouseOnUIWhenClicked = container.doesMIntExist();
        //clickOnUI();
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

void NonSimulationStuff::mouseUIEvents(sf::Event &event)
{
    //std::cout << mouseOnButtonWhenClicked.first << "\n";
    //if(!mouseOnButtonWhenClicked.first)
        if(event.type == sf::Event::MouseButtonPressed)
        {
            //std::cout << "hello\n";
            clickedWindowToDrag = container.isWindowDraggable();
        }
    /*else if(event.type == sf::Event::MouseButtonReleased)
    {
        //std::cout << "declick\n";
        container.resetUIClick();
        clickedWindowToDrag = false;
        container.checkMouseIntersection(window, GUIView, worldView);
    }*/
}

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
    /*else if((!newLayerKeys[0])&&newLayerKeys[1])
    {
        std::cout << "HEllo\n";
        spawnRadius += 5;
        if(event.key.code == sf::Keyboard::R)
            std::cout << "Hello2\n";
        else if(event.key.code == sf::Keyboard::M)
            spawnMass += 5;
    }
    else if(newLayerKeys[0]&&newLayerKeys[1])
    {
        std::cout << "Hello3\n";
        if(event.key.code == sf::Keyboard::R)
            spawnRadius -= 5;
        else if(event.key.code == sf::Keyboard::M)
            spawnMass -= 5;
    }*/
    else if(newLayerKeys[2])
    {
        if(event.key.code == sf::Keyboard::Return)
            toggleFullScreen();
    }
}

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
                ballSim.decSimStep(0.1);
            else if(event.key.code == sf::Keyboard::Period)
                ballSim.incSimStep(0.1);
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

void NonSimulationStuff::resizeEvents(sf::Event &event)
{
    if(event.type == sf::Event::Resized)
        adjustViewSize(event.size.width, event.size.height, wSize.x, wSize.y);//, currentZoom);
}

void NonSimulationStuff::incTimeStep(sf::Time delta)
{
    if(delta>sf::milliseconds(0))
        timestep+=delta;
}
void NonSimulationStuff::decTimeStep(sf::Time delta)
{
    if(delta>sf::milliseconds(0) && timestep>delta)
        timestep-=delta;
}

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

    container.addWindow({0,150}, 250, 250, true, true);
    container.addWindow({0,0}, 250, 50, true, false);
    container.addWindow({0,450}, 250, 50, false, true);

    container.getWindow(0).addElement("./fonts/cour.ttf", "No. Balls:", 16, {1,1}, &ballSim.getNumOfBalls());
    container.getWindow(0).addElement("./fonts/cour.ttf", "Spawn Mass:", 16, {00,30}, &spawnMass);
    container.getWindow(0).addElement("./fonts/cour.ttf", "Spawn Radius:", 16, {00,50}, &spawnRadius);
    container.getWindow(0).addElement("./fonts/cour.ttf", "Forces Enabled:", 16, {0,70}, &ballSim.getForcesEnabled());
    container.getWindow(0).addElement("./fonts/cour.ttf", "Collisions Enabled:", 16, {0,90}, &ballSim.getCollisionsEnabled());
    container.getWindow(0).addButton("./fonts/cour.ttf", "Mass +", 12, {10,140}, {60,30}, [&]{spawnMass+=1;});
    container.getWindow(0).addButton("./fonts/cour.ttf", "Mass -", 12, {90,140}, {60,30}, [&]{if(spawnMass>1){spawnMass-=1;}});
    container.getWindow(0).addButton("./fonts/cour.ttf", "Rst Mass", 12, {170,140}, {60,30}, [&]{spawnMass=1;});
    container.getWindow(0).addButton("./fonts/cour.ttf", "Rad +", 12, {10,180}, {60,30}, [&]{spawnRadius+=1;});
    container.getWindow(0).addButton("./fonts/cour.ttf", "Rad -", 12, {90,180}, {60,30}, [&]{if(spawnRadius>1){spawnRadius-=1;}});
    container.getWindow(0).addButton("./fonts/cour.ttf", "Rst Rad", 12, {170,180}, {60,30}, [&]{spawnRadius=10;});

    /*std::string font, std::string text, int fontSize, sf::Vector2f position, sf::Vector2f bSize,
                                                bool fixedToWin, std::function<void> *func, sf::Color color*/

    container.getWindow(1).addElement("./fonts/cour.ttf", "WindowSizeX:", 16, {00,00}, &windowSizeX);
    container.getWindow(1).addElement("./fonts/cour.ttf", "WindowSizeY:", 16, {0,20}, &windowSizeY);
    //container.getWindow(1).addButton("./fonts/cour.ttf", "Rad -", 16, {0,50}, {80,40}, increaseMass);

    container.getWindow(2).addButton("./fonts/cour.ttf", "Star", 12, {10,10}, {60,30}, [&]{spawnRadius=50;spawnMass=100;});
    container.getWindow(2).addButton("./fonts/cour.ttf", "Planet", 12, {90,10}, {60,30}, [&]{spawnRadius=10;spawnMass=1;});
    container.getWindow(2).addButton("./fonts/cour.ttf", "Asteroid", 12, {170,10}, {60,30}, [&]{spawnRadius=3;spawnMass=0.01;});



}
