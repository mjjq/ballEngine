#include "classGameObjectManager.h"

GameObjectManager::GameObjectManager()
{
    GameObject::engineNotify.addObserver(this);
}

GameObjectManager::~GameObjectManager()
{
    std::cout << gameObjects.size() << "\n";
    for(int i=0; i<(int)gameObjects.size(); ++i)
        delete gameObjects[i];

    gameObjects.clear();
    GameObject::engineNotify.removeObserver(this);
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
            gameObjects.erase(gameObjects.begin()+i);
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
            //gameObjects.erase(gameObjects.begin() + index);
        }
        else if(index < 0)
        {
            delete gameObjects[gameObjects.size() + index];
            //gameObjects.erase(gameObjects.begin() + gameObjects.size() + index);
        }
    }
    else if(index == -1 && gameObjects.size() == 1)
    {
        delete gameObjects[0];
        gameObjects.clear();
    }
}

void GameObjectManager::clearAll()
{
    /*for(int i=0; i<(int)gameObjects.size(); ++i)
    {
        delete gameObjects[0];
    }*/
    while(gameObjects.size() > 0)
    {
        delete gameObjects[0];
    }
    gameObjects.clear();
}

void GameObjectManager::onNotify(Component& entity, Event event, Container* data)
{
    switch(event.type)
    {
        case(EventType::New_GameObject):
            addObject((GameObject*)(&entity));
            break;
        case(EventType::Delete_GameObject):
            removeObject((GameObject*)(&entity));
            break;
        default:
            break;
    }
}
