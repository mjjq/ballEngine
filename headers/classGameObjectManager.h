#ifndef CLASS_GAMEOBJECTMAN_H
#define CLASS_GAMEOBJECTMAN_H

#include "classGameObject.h"

class GameObjectManager
{
public:
    ~GameObjectManager();
    ProjectileType PROJ_TYPE = ProjectileType::Bullet;
    std::vector<GameObject* > projectiles;
    void addObject(GameObject* proj);
    void removeObject(GameObject* proj);
    void removeObject(int index);
};

#endif // CLASS_GAMEOBJECTMAN_H
