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

#include "classPauseMenuScene.h"
#include "Math.h"
#include "stringConversion.h"


void PauseMenuScene::load()
{
    adjustViewSize(window.getSize());

    buttonFuncMap = {
        {"resume", [&]{requestScene(prevScene);}},
        {"options", [&]{}},
        {"exit", [&]{window.close();}},
        {"mainMenu", [&]{requestScene(SceneEnum::SCENE_MENU);}}
    };

    loadUI("pausemenusceneUI.json", container);
    loadKeybinds("keybinds.json", "PauseScene");

}

PauseMenuScene::PauseMenuScene(sf::RenderWindow &_window) : MenuScene(_window)
{

}


