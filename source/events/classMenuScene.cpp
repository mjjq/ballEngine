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

#include "../../headers/classMenuScene.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"



void MenuScene::events(sf::Event &event)
{
    mouseViewEvents(event);

    if(mouseOnUIWhenClicked.first)
        mouseUIEvents(event);

    keyEvents(event);
    resizeEvents(event);
}

void MenuScene::load()
{
    adjustViewSize(window.getSize());

    buttonFuncMap = {
        {"newGame", [&]{requestScene(SceneEnum::SCENE_GAME);}},
        {"options", [&]{}},
        {"exit", [&]{window.close();}}
    };

    loadUI("./json/menusceneUI.json", container);
    loadKeybinds("./json/keybinds.json", "MenuScene");

}

void MenuScene::unload()
{
    container.destroyAllWindows();
}

void MenuScene::redraw(sf::RenderWindow &_window)
{
    container.renderWindows(_window, GUIView, worldView);
}

void MenuScene::update(sf::RenderWindow &_window)
{
    if(!mouseOnUIWhenClicked.first)
    {
    }
    if(clickedWindowToDrag)
        container.dragWindow(_window);

    exePressedKeys();
}

MenuScene::MenuScene(sf::RenderWindow &_window) : Scene(_window)
{

}


