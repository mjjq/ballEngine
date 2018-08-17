#include <iostream>
#include <SFML/Graphics.hpp>

#include "../../headers/classScene.h"

void Scene::requestScene(SceneEnum scEnum)
{
    nextScene = scEnum;
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
    std::ifstream input(filePath);
    if(json::accept(input))
    {
        std::ifstream input(filePath);
        json j;
        input >> j;
        for(json &winJ : j["Windows"])
            container.addWindow(winJ, buttonFuncMap, sliderFuncMap, textVarMap);
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
