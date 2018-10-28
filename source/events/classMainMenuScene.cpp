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

#include "../../headers/classMainMenuScene.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"


void MainMenuScene::load()
{
    adjustViewSize(window.getSize());

    buttonFuncMap = {
        {"newSandbox",  [&]{requestScene(SceneEnum::SCENE_SANDBOX);}},
        {"newSvival",   [&]{requestScene(SceneEnum::SCENE_SURVIVAL);}},
        {"options",     [&]{}},
        {"exit",        [&]{window.close();}}
    };

    loadUI("./json/menusceneUI.json", container);
    loadKeybinds("./json/keybinds.json", "MenuScene");

}

MainMenuScene::MainMenuScene(sf::RenderWindow &_window) : MenuScene(_window)
{

}


