#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>
#include <numeric>

#include "classSceneManager.h"


SceneManager::SceneManager()
{
    //UIWindow::appWinSize = windowSize;
}

void SceneManager::mainLoop()
{
    //UIWindow::appWinSize = windowSize;
    WindowManager& winMan = sceneRenderer->windowManager;
    sf::RenderWindow& currWin = sceneRenderer->windowManager.getWindow();
    currScene->load();
    sceneRenderer->windowManager.limitFramerate(60);
    currWin.setKeyRepeatEnabled(false);

    while(currWin.isOpen())
    {
        winMan.getFrameClock().restart();
        currWin.clear();

        sf::Event event;
        while(currWin.pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed)
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)
                && event.key.code == sf::Keyboard::Return)
                    winMan.toggleFullScreen(*currScene);
                else if(event.key.code == sf::Keyboard::F2)
                    winMan.setAALevel(0, *currScene);
                else if(event.key.code == sf::Keyboard::F3)
                    winMan.setAALevel(8, *currScene);
                else if(event.key.code == sf::Keyboard::End)
                    winMan.limitFramerate(240);
                else if(event.key.code == sf::Keyboard::Home)
                    winMan.limitFramerate(60);
                else if(event.key.code == sf::Keyboard::PageDown)
                    winMan.limitFramerate(30);
                //else if(event.key.code == sf::Keyboard::Escape)
                //    currScene->requestScene(SceneEnum::SCENE_MENU);
            }
            else if(event.type == sf::Event::Closed)
                currWin.close();

            else if(event.type == sf::Event::Resized)
                sceneRenderer->resizeWindow({event.size.width, event.size.height});

            currScene->events(event);
        }

        currScene->update(currWin);
        sceneRenderer->redrawAll(currWin);
        currScene->redraw(currWin);
        currWin.display();

        nextSceneEnum = currScene->pollNextScene();

        if(nextSceneEnum!=SceneEnum::LAST && nextSceneEnum!=thisSceneEnum)
            loadNextScene(nextSceneEnum);

        winMan.updateFPS(sf::seconds(0.5f));
    }
}


void SceneManager::loadNextScene(SceneEnum nextScene)
{
    //std::cout << static_cast<int>(nextScene) << "\n";
    currScene->requestScene(SceneEnum::LAST);
    switch(nextScene)
    {
        case(SceneEnum::SCENE_SANDBOX):
            currScene->unload();
            currScene = &sandbox;
            gameScene = currScene;
            break;
        case(SceneEnum::SCENE_SURVIVAL):
            currScene->unload();
            currScene = &survival;
            gameScene = currScene;
            break;
        case(SceneEnum::SCENE_MENU):
            currScene->unload();
            currScene = &menu;
            gameScene->unload();
            break;
        case(SceneEnum::SCENE_PAUSEMENU):
            currScene = &pauseMenu;
            break;
        case(SceneEnum::LAST):
            break;
        default:
            break;
    }
    currScene->setPrevScene(thisSceneEnum);
    thisSceneEnum = nextSceneEnum;
    currScene->load();
    currScene->adjustViewSize(sceneRenderer->windowManager.getWindow().getSize());
}
