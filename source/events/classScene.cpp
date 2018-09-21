#include <iostream>
#include <SFML/Graphics.hpp>

#include "../../headers/classScene.h"

void Scene::requestScene(SceneEnum scEnum)
{
    nextScene = scEnum;
    pressedKeyStack.clear();
}

SceneEnum Scene::pollNextScene()
{
    return nextScene;
}

void Scene::update(sf::RenderWindow &window)
{

}

void Scene::redraw(sf::RenderWindow &window)
{

}

void Scene::initBindings()
{

}

void Scene::loadUI(std::string filePath, UIContainer &container)
{
    using json = nlohmann::json;
    std::ifstream initInput(filePath);
    if(json::accept(initInput))
    {
        std::ifstream input(filePath);
        json j;
        input >> j;
        for(json &winJ : j["Windows"])
            container.addWindow(winJ, buttonFuncMap, sliderFuncMap, textVarMap);
    }
}

void Scene::loadKeybinds(std::string filePath, std::string sceneType)
{
    KeyBinds::loadKeybinds(filePath, sceneType, buttonFuncMap, keyBinds);
}

void Scene::load()
{

}

void Scene::unload()
{

}

/**
    Scales the view to the window based on the current window and world sizes.
    This function ensure the world level zoom is preserved as well as accounting
    for vertical aspect ratios by checking for effective zooms in each direction.

    @param sizeX The x-direction size of the window.
    @param sizeY The y-direction size of the window.
    @param worldSizeX The x-direction size of the simulation world.
    @param worldSizeY The y-direction size of the simulation world.

    @return Void.
*/
void Scene::adjustViewSize(sf::Vector2u newSize)
{
    worldView.setSize(newSize.x, newSize.y);
    GUIView.setSize(newSize.x, newSize.y);

    window.setView(worldView);
}

void Scene::events(sf::Event &event)
{

}


/**
    Function which handles general key based events. Events are processed provided
    there are no key primary keys held. If these keys are held, the newLayerEvent
    events are processed instead.

    @param &event The event case to process.

    @return Void.
*/
void Scene::keyEvents(sf::Event &event)
{
    if(event.type == sf::Event::EventType::KeyPressed)
        pressedKeyStack.push_back(event.key.code);

    else if(event.type == sf::Event::EventType::KeyReleased)
    {
        pressedKeyStack.erase(std::remove(pressedKeyStack.begin(),
                                          pressedKeyStack.end(), event.key.code),
                              pressedKeyStack.end());
    }
}


void Scene::exePressedKeys()
{
    std::vector<sf::Keyboard::Key > tempStack = pressedKeyStack;
    bool functionFound = false;
    while(tempStack.size() > 0 && functionFound == false)
    {
        if(tempStack.size()>1)
        {
            if(keyBinds.find(tempStack) != keyBinds.end())
            {
                keyBinds[tempStack]();
                functionFound = true;
                pressedKeyStack.pop_back();
            }
            else
            {
                tempStack.pop_back();
            }
        }

        else if(tempStack.size() == 1)
        {
            for(unsigned int i=0; i<pressedKeyStack.size(); ++i)
            {
                if(keyBinds.find({pressedKeyStack.at(i)}) != keyBinds.end())
                {
                    keyBinds[{pressedKeyStack.at(i)}]();
                    functionFound = true;
                }
            }
            tempStack.pop_back();
        }
    }
}


/**
    Clicks on a User Interface window.

    @return Void.
*/
void Scene::clickOnUI()
{
    container.checkMouseIntersection(window, GUIView, worldView);
    mouseOnUIWhenClicked = container.doesMIntExist();
    container.clickOnUI(window);

}

/**
    Sets the current User Interface window to not be draggable.

    @return Void.
*/
void Scene::resetUIClick()
{
    container.resetUIClick();
    clickedWindowToDrag = false;
}


/**
    Function which handles all events to do with checking for UI interaction
    or adjusting the view.

    @param &event The event case to process.

    @return Void.
*/
void Scene::mouseViewEvents(sf::Event &event)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        clickOnUI();
    }

    else if(event.type == sf::Event::MouseMoved)
    {
        container.checkMouseIntersection(window, GUIView, worldView);
        container.doesMIntExist();
    }

    else if(event.type == sf::Event::MouseButtonReleased)
    {
        resetUIClick();
    }

}


/**
    Function which handles all events to do with generating interactions with
    the User Interface.

    @param &event The event case to process.

    @return Void.
*/
void Scene::mouseUIEvents(sf::Event &event)
{
    if(event.type == sf::Event::MouseButtonPressed)
        clickedWindowToDrag = container.isWindowDraggable();
}


/**
    Function which handles events on window resize.

    @param &event The event case to process.

    @return Void.
*/
void Scene::resizeEvents(sf::Event &event)
{
    if(event.type == sf::Event::Resized)
        adjustViewSize({event.size.width, event.size.height});//, currentZoom);
}

Scene::Scene(sf::RenderWindow &_window) : window{_window}
{

}
