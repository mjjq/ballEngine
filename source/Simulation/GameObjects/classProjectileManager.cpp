#include "classProjectileManager.h"
ProjectileManager::~ProjectileManager()
{
    std::cout << projectiles.size() << "\n";
    for(int i=0; i<projectiles.size(); ++i)
        delete projectiles[i];

    projectiles.clear();
}

void ProjectileManager::addProjectile(Projectile* proj)
{
    projectiles.push_back(std::move(proj));
}

void ProjectileManager::removeProjectile(Projectile* proj)
{
    int i=0;
    bool projFound = false;

    while(i<(int)projectiles.size() && !projFound)
    {
        if(projectiles[i] == proj)
        {
            removeProjectile(i);
        }
        ++i;
    }
}

void ProjectileManager::removeProjectile(int index)
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
