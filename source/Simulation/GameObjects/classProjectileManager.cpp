#include "classProjectileManager.h"

void ProjectileManager::addProjectile(Projectile* proj)
{
    projectiles.push_back(std::move(proj));
}

void ProjectileManager::removeProjectile(Projectile* proj)
{
    for(int i=0; i<(int)projectiles.size(); ++i)
    {
        if(projectiles[i] == proj)
        {
            projectiles.erase(projectiles.begin() + i);
            break;
        }
    }
}
