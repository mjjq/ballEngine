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

#include "../../headers/classPauseMenuScene.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"



void PauseMenuScene::events(sf::Event &event)
{
    mouseViewEvents(event);

    if(mouseOnUIWhenClicked.first)
        mouseUIEvents(event);

    keyEvents(event);
    resizeEvents(event);
}

void PauseMenuScene::load()
{
    adjustViewSize(window.getSize());

    buttonFuncMap = {
        {"resume", [&]{requestScene(SceneEnum::SCENE_GAME);}},
        {"options", [&]{}},
        {"exit", [&]{window.close();}},
        {"mainMenu", [&]{requestScene(SceneEnum::SCENE_MENU);}}
    };

    loadUI("./json/pausemenusceneUI.json", container);
    loadKeybinds("./json/keybinds.json", "PauseScene");

}

void PauseMenuScene::unload()
{
    container.destroyAllWindows();
}

void PauseMenuScene::redraw(sf::RenderWindow &_window)
{
    container.renderWindows(_window, GUIView, worldView);
}

void PauseMenuScene::update(sf::RenderWindow &_window)
{
    if(!mouseOnUIWhenClicked.first)
    {
    }
    if(clickedWindowToDrag)
        container.dragWindow(_window);

    exePressedKeys();

}

PauseMenuScene::PauseMenuScene(sf::RenderWindow &_window) : Scene(_window)
{

}


