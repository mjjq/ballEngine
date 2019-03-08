#include "classGameObjectManager.h"

GameObjectManager::~GameObjectManager()
{
    std::cout << gameObjects.size() << "\n";
    for(int i=0; i<gameObjects.size(); ++i)
        delete gameObjects[i];

    gameObjects.clear();
}

void GameObjectManager::addObject(GameObject* proj)
{
    gameObjects.push_back(std::move(proj));
}

void GameObjectManager::removeObject(GameObject* proj)
{
    int i=0;
    bool projFound = false;

    while(i<(int)gameObjects.size() && !projFound)
    {
        if(gameObjects[i] == proj)
        {
            removeObject(i);
        }
        ++i;
    }
}

void GameObjectManager::removeObject(int index)
{
    if(std::abs(index) < (int)gameObjects.size())
    {
        if(index >=0)
        {
            delete gameObjects[index];
            gameObjects.erase(gameObjects.begin() + index);
        }
        else if(index < 0)
        {
            delete gameObjects[gameObjects.size() + index];
            gameObjects.erase(gameObjects.begin() + gameObjects.size() + index);
        }
    }
    else if(index == -1 && gameObjects.size() == 1)
    {
        delete gameObjects[0];
        gameObjects.clear();
    }
}
