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

void Scene::loadKeybinds(std::string filePath, StringFuncMap &sfMap, KeyFuncMap &keyMap)
{
    using json = nlohmann::json;
    std::ifstream initInput(filePath);
    if(json::accept(initInput))
    {
        std::ifstream input(filePath);
        json j;
        input >> j;
        keyMap = KeyBinds::createMapFromJSON(j, sfMap);
    }
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
