#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>
#include <numeric>

#include "../../headers/classSceneManager.h"

void SceneManager::mainLoop()
{
    game.load();
    limitFramerate(60);
    window.setKeyRepeatEnabled(false);

    while(window.isOpen())
    {
        frameClock.restart();
        window.clear();

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed)
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)
                && event.key.code == sf::Keyboard::Return)
                    toggleFullScreen(game);
                else if(event.key.code == sf::Keyboard::F2)
                    setAALevel(0, game);
                else if(event.key.code == sf::Keyboard::F3)
                    setAALevel(8, game);
                else if(event.key.code == sf::Keyboard::F5)
                    game.unload();
            }
            else if(event.type == sf::Event::Closed)
                window.close();

            game.events(event);
        }

        game.update(window);
        game.redraw(window);
        window.display();

        updateFPS(sf::seconds(0.5f), 1.0f/currentFrameTime.asSeconds());
        timeSinceFSample+=currentFrameTime;
        currentFrameTime = sampleNextFrame(frameClock.getElapsedTime());
    }
}


void SceneManager::limitFramerate(int framerate)
{
    window.setFramerateLimit(framerate);
    targetFTime = sf::seconds(1.0f/static_cast<float>(framerate));
    previousFrames.clear();
    previousFrames.push_back(targetFTime.asSeconds());
}


/**
    Stores the current frametime to a std::vector containing the
    "history" of frametimes. Also erases the oldest entry in the
    history vector given by positionSize.

    @return Void.
*/
sf::Time SceneManager::sampleNextFrame(sf::Time frameTime)
{
    unsigned int positionSize = 10;
    previousFrames.push_back(frameTime.asSeconds());
    if(previousFrames.size()>positionSize)
    {
        int eraseUpperLimit = previousFrames.size() - positionSize;
        previousFrames.erase(previousFrames.begin(),
                                previousFrames.begin()+eraseUpperLimit);
    }
    return sf::seconds(std::accumulate(previousFrames.begin(),
                                       previousFrames.end(), 0.0f)/
                           static_cast<float>(previousFrames.size()));

}


void SceneManager::updateFPS(sf::Time interval, float framerate)
{
    if(timeSinceFSample >= interval)
    {
        currentFPS = framerate;
        timeSinceFSample = sf::milliseconds(0);
    }
}


/**
    Toggles the window between windowed and fullscreen mode.

    @return Void.
*/
void SceneManager::toggleFullScreen(Scene &currScene)
{
    //setAALevel(8);
    if(!isFullScreen)
    {
        prevWinSize = windowSize;
        prevWinPos = window.getPosition();

        sf::VideoMode mode = sf::VideoMode::getDesktopMode();
        window.setSize({mode.width,mode.height});
        window.create(sf::VideoMode(mode.width,mode.height), "ballEngine", sf::Style::None, settings);
        currScene.adjustViewSize({mode.width, mode.height});
        isFullScreen = true;
    }
    else
    {
        //sf::Vector2u prevSize = sf::Vector2u(prevWindowSizeX,prevWindowSizeY);
        window.setSize(prevWinSize);
        window.create(sf::VideoMode(prevWinSize.x, prevWinSize.y), "ballEngine");
        currScene.adjustViewSize({prevWinSize.x, prevWinSize.y});
        window.setPosition(prevWinPos);
        isFullScreen = false;
    }
    limitFramerate(static_cast<int>(1.0f/targetFTime.asSeconds()));
    window.setKeyRepeatEnabled(false);
}


void SceneManager::setAALevel(unsigned int level, Scene &currScene)
{
    settings.antialiasingLevel = level;

    prevWinSize = windowSize;
    prevWinPos = window.getPosition();
    if(isFullScreen)
    {
        sf::VideoMode mode = sf::VideoMode::getDesktopMode();
        window.setSize({mode.width,mode.height});
        window.create(sf::VideoMode(mode.width,mode.height), "ballEngine", sf::Style::None, settings);
        currScene.adjustViewSize({mode.width, mode.height});
        isFullScreen = true;
    }
    else
    {
        prevWinSize = windowSize;
        prevWinPos = window.getPosition();
        //sf::Vector2u prevSize = sf::Vector2u(prevWindowSizeX,prevWindowSizeY);
        window.setSize(prevWinSize);
        window.create(sf::VideoMode(prevWinSize.x, prevWinSize.y), "ballEngine");
        currScene.adjustViewSize({prevWinSize.x, prevWinSize.y});
        window.setPosition(prevWinPos);
        isFullScreen = false;
    }
    limitFramerate(static_cast<int>(1.0f/targetFTime.asSeconds()));
    window.setKeyRepeatEnabled(false);
}