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

    KeyBinds::keyEvents(event, pressedKeyStack, releasedKeyStack);
    resizeEvents(event);
}

void MenuScene::load()
{

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
    if(clickedWindowToDrag)
        container.dragWindow(_window);

    KeyBinds::exePressedKeys(pressedKeyStack, keyBinds);
    window.setMouseCursorVisible(true);
}

MenuScene::MenuScene(sf::RenderWindow &_window) : Scene(_window)
{

}


