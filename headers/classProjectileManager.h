#ifndef CLASS_PROJMAN_H
#define CLASS_PROJMAN_H

#include "classProjectile.h"

class ProjectileManager
{
public:
    ProjectileType PROJ_TYPE = ProjectileType::Bullet;
    std::vector<Projectile* > projectiles;
    void addProjectile(Projectile* proj);
    void removeProjectile(Projectile* proj);
};

#endif // CLASS_PROJMAN_H
