#include "windowManager.h"

void WindowManager::limitFramerate(int framerate)
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
sf::Time WindowManager::sampleNextFrame(sf::Time frameTime)
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

void WindowManager::updateFPS(sf::Time interval)
{
    updateFPS(interval, 1.0f/getCurrFrameTime().asSeconds());
}

void WindowManager::updateFPS(sf::Time interval, float framerate)
{
    if(timeSinceFSample >= interval)
    {
        currentFPS = framerate;
        timeSinceFSample = sf::milliseconds(0);
    }

    timeSinceFSample+=currentFrameTime;
    currentFrameTime = sampleNextFrame(frameClock.getElapsedTime());
}


/**
    Toggles the window between windowed and fullscreen mode.

    @return Void.
*/
void WindowManager::toggleFullScreen(Scene &_currScene)
{
    //setAALevel(8);
    if(!isFullScreen)
    {
        prevWinSize = windowSize;
        prevWinPos = window.getPosition();

        sf::VideoMode mode = sf::VideoMode::getDesktopMode();
        window.setSize({mode.width,mode.height});
        window.create(sf::VideoMode(mode.width,mode.height), "ballEngine", sf::Style::None, settings);
        _currScene.adjustViewSize({mode.width, mode.height});
        isFullScreen = true;
    }
    else
    {
        //sf::Vector2u prevSize = sf::Vector2u(prevWindowSizeX,prevWindowSizeY);
        window.setSize(prevWinSize);
        window.create(sf::VideoMode(prevWinSize.x, prevWinSize.y), "ballEngine");
        _currScene.adjustViewSize({prevWinSize.x, prevWinSize.y});
        window.setPosition(prevWinPos);
        isFullScreen = false;
    }
    limitFramerate(static_cast<int>(1.0f/targetFTime.asSeconds()));
    window.setKeyRepeatEnabled(false);
}


void WindowManager::setAALevel(unsigned int level, Scene &_currScene)
{
    settings.antialiasingLevel = level;

    prevWinSize = windowSize;
    prevWinPos = window.getPosition();
    if(isFullScreen)
    {
        sf::VideoMode mode = sf::VideoMode::getDesktopMode();
        window.setSize({mode.width,mode.height});
        window.create(sf::VideoMode(mode.width,mode.height), "ballEngine", sf::Style::None, settings);
        _currScene.adjustViewSize({mode.width, mode.height});
        isFullScreen = true;
    }
    else
    {
        prevWinSize = windowSize;
        prevWinPos = window.getPosition();
        //sf::Vector2u prevSize = sf::Vector2u(prevWindowSizeX,prevWindowSizeY);
        window.setSize(prevWinSize);
        window.create(sf::VideoMode(prevWinSize.x, prevWinSize.y), "ballEngine");
        _currScene.adjustViewSize({prevWinSize.x, prevWinSize.y});
        window.setPosition(prevWinPos);
        isFullScreen = false;
    }
    limitFramerate(static_cast<int>(1.0f/targetFTime.asSeconds()));
    window.setKeyRepeatEnabled(false);
}

sf::RenderWindow& WindowManager::getWindow()
{
    return window;
}

sf::Time& WindowManager::getCurrFrameTime()
{
    return currentFrameTime;
}

sf::Time& WindowManager::getTargFrameTime()
{
    return targetFTime;
}

float& WindowManager::getCurrentFPS()
{
    return currentFPS;
}

sf::Clock& WindowManager::getFrameClock()
{
    return frameClock;
}

sf::Vector2u WindowManager::getWindowSize()
{
    return window.getSize();
}
