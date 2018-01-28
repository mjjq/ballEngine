#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "classes.h"
#include "sfVectorMath.h"

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
    float zoom = currentZoom;
    float winSizeX = window.getSize().x;
    float winSizeY = window.getSize().y;
    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
    float xFactor = zoom*static_cast<float>(worldSizeX)/static_cast<float>(winSizeX);
    float yFactor = zoom*static_cast<float>(worldSizeY)/static_cast<float>(winSizeY);
    sf::Vector2f effectiveZoom = {xFactor,yFactor};
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

void NonSimulationStuff::adjustViewSize(int sizeX, int sizeY, int worldSizeX, int worldSizeY, float zoom)
{
    sf::Vector2f effectiveZoom = getEffectiveZoom(worldSizeX,worldSizeY);
    worldView.setSize(sizeX,sizeY);
    windowSizeX = sizeX;
    windowSizeY = sizeY;

    if(effectiveZoom.y > effectiveZoom.x)
        worldView.zoom(effectiveZoom.y);
    else
        worldView.zoom(effectiveZoom.x);

    window.setView(worldView);
}

void NonSimulationStuff::resetView()
{
    currentZoom = 1.0;
    sf::Vector2i wSize = ballSim.getWorldSize();
    worldView.setCenter(wSize.x/2,wSize.y/2);
    adjustViewSize(window.getSize().x, window.getSize().y, wSize.x, wSize.y, currentZoom);
    window.setView(worldView);
}

void NonSimulationStuff::drawBalls()
{
    std::vector<Ball>* arrayAddress = ballSim.getBallArrayAddress();
    for(int i=0; i<arrayAddress->size(); i++)
        window.draw(arrayAddress->at(i));
}

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

void NonSimulationStuff::mainLoop()
{
    sf::Vector2i mousePosOnClick;
    sf::Vector2i mousePosOnRelease;
    sf::Vector2f recentViewCoords;
    sf::Vector2i wSize = ballSim.getWorldSize();

    while(window.isOpen())
    {
        window.clear();
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i viewPos = sf::Mouse::getPosition(window);
                mousePosOnClick = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));
            }
            window.setKeyRepeatEnabled(false);
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                //std::cout << "thing\n";
                recentViewCoords = worldView.getCenter();
                mousePosOnClick = sf::Mouse::getPosition(window);
                //mousePosOnClick = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));
            }
            //window.setKeyRepeatEnabled(true);

            if(event.type == sf::Event::EventType::MouseButtonReleased
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

            if(event.type == sf::Event::EventType::MouseButtonReleased
                            && event.mouseButton.button==sf::Mouse::Middle
                            && !(timeToNextSpawn > sf::milliseconds(0)))
            {
                sf::Vector2f velocity = velocityFromMouse(mousePosOnClick, spawnVelFactor);
                ballSim.createBallGrid(ballGridWidth,ballGridHeight,ballGridSpacing,
                                static_cast<sf::Vector2f>(mousePosOnClick),velocity,spawnMass,spawnRadius);
            }

            if(event.type == sf::Event::Resized)
            {
                //sf::Vector2i wSize = ballSim.getWorldSize();
                adjustViewSize(event.size.width, event.size.height, wSize.x, wSize.y, currentZoom);
            }

            if(event.type == sf::Event::MouseWheelScrolled)
            {
                float delta = event.mouseWheelScroll.delta;
                mouseWheelZoom(sf::Keyboard::isKeyPressed(sf::Keyboard::Space), delta);
            }

            if(event.type == sf::Event::EventType::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::PageUp)
                    timestep+=sf::milliseconds(10);
                else if(event.key.code == sf::Keyboard::PageDown && timestep>sf::milliseconds(15))
                    timestep-=sf::milliseconds(10);
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
                /*else if(event.key.code == sf::Keyboard::Space)
                {
                    std::cout << "thing\n";
                    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
                    mousePosOnClick = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));
                }*/
            }
        }

        checkMBPress(mousePosOnClick,sf::Mouse::isButtonPressed(sf::Mouse::Left));
        checkMBPress(mousePosOnClick,sf::Mouse::isButtonPressed(sf::Mouse::Middle));
        checkMBPress(mousePosOnClick,sf::Mouse::isButtonPressed(sf::Mouse::Right));
        checkForViewPan(mousePosOnClick,recentViewCoords, wSize.x, wSize.y, sf::Keyboard::isKeyPressed(sf::Keyboard::Space));

        ballSim.universeLoop();
        drawBalls();
        window.draw(boundaryRect);

        window.display();
        sf::sleep(timestep);
        timeToNextSpawn -= timestep;
    }
}

NonSimulationStuff::NonSimulationStuff(int m_windowSizeX, int m_windowSizeY, int spawnVelFactor,
                 float spawnRadius, float spawnMass, float ballGridSpacing, int ballGridHeight,
                                    int ballGridWidth, BallUniverse sim) :

windowSizeX{m_windowSizeX}, windowSizeY{m_windowSizeY}, spawnVelFactor{spawnVelFactor},
            spawnRadius{spawnRadius}, spawnMass{spawnMass}, ballGridSpacing{ballGridSpacing},
            ballGridHeight{ballGridHeight}, ballGridWidth{ballGridWidth}, ballSim{sim}
{
    sf::Vector2i wSize = ballSim.getWorldSize();
    changeBoundaryRect(wSize);
    resetView();
}
