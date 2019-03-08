#include "classGameObjectManager.h"

GameObjectManager::~GameObjectManager()
{
    std::cout << projectiles.size() << "\n";
    for(int i=0; i<projectiles.size(); ++i)
        delete projectiles[i];

    projectiles.clear();
}

void GameObjectManager::addObject(GameObject* proj)
{
    projectiles.push_back(std::move(proj));
}

void GameObjectManager::removeObject(GameObject* proj)
{
    int i=0;
    bool projFound = false;

    while(i<(int)projectiles.size() && !projFound)
    {
        if(projectiles[i] == proj)
        {
            removeObject(i);
        }
        ++i;
    }
}

void GameObjectManager::removeObject(int index)
{
    if(std::abs(index) < (int)projectiles.size())
    {
        if(index >=0)
        {
            delete projectiles[index];
            projectiles.erase(projectiles.begin() + index);
        }
        else if(index < 0)
        {
            delete projectiles[projectiles.size() + index];
            projectiles.erase(projectiles.begin() + projectiles.size() + index);
        }
    }
    else if(index == -1 && projectiles.size() == 1)
    {
        delete projectiles[0];
        projectiles.clear();
    }
}
