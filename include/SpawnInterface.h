#ifndef SPAWN_INTERFACE_H
#define SPAWN_INTERFACE_H

#include "classUniverse.h"
#include "classPhysicsObject.h"

class SpawnInterface
{
    BallUniverse* worldAddress;
public:
    void getWorldAddress(BallUniverse* address);

    void spawnNewObject(bool isStatic,
                        SpawnObjectType type,
                        ObjectProperties init);
};

#endif // SPAWN_INTERFACE_H
