#ifndef CLASS_GAMEOBJECTMAN_H
#define CLASS_GAMEOBJECTMAN_H

#include "classGameObject.h"

class GameObjectManager : public Observer
{
    void addObject(GameObject* proj);
    void removeObject(GameObject* proj);
public:
    GameObjectManager();
    ~GameObjectManager();
    std::vector<GameObject* > gameObjects;
    void removeObject(int index);
    void clearAll();
    void onNotify(Entity& entity, Event event);
};

#endif // CLASS_GAMEOBJECTMAN_H
