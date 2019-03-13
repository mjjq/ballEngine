#ifndef CLASS_GAMEOBJECTMAN_H
#define CLASS_GAMEOBJECTMAN_H

#include "classGameObject.h"

class GameObjectManager
{
public:
    ~GameObjectManager();
    std::vector<GameObject* > gameObjects;
    void addObject(GameObject* proj);
    void removeObject(GameObject* proj);
    void removeObject(int index);
    void clearAll();
};

#endif // CLASS_GAMEOBJECTMAN_H
