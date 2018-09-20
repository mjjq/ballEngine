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

void Scene::adjustViewSize(sf::Vector2u newSize)
{

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

        else if(tempStack.size() == 1);
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
