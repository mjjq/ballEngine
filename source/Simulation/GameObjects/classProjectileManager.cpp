#include "classProjectileManager.h"

void ProjectileManager::addProjectile(Projectile* proj)
{
    projectiles.push_back(std::move(proj));
}

void ProjectileManager::removeProjectile(Projectile* proj)
{
    int i=0;
    bool projFound = false;

    while(i<projectiles.size() && !projFound)
    {
        if(projectiles[i] == proj)
        {
            projectiles.erase(projectiles.begin() + i);
            projFound = true;
        }
        ++i;
    }
}
