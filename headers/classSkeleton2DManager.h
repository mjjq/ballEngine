#ifndef CLASS_SKEL_2D_MAN_H
#define CLASS_SKEL_2D_MAN_H

#include "classSkeleton2DWrap.h"

class Skeleton2DManager : public Observer
{
    std::vector<Skeleton2DWrap* > skeletons;

public:
    Skeleton2DManager();
    ~Skeleton2DManager();
    void updateAll(float deltaTime);

    void addSkeleton(Skeleton2DWrap* skel);
    void removeSkeleton(Skeleton2DWrap* skel);
    void removeSkeleton(int index);
    void clearAll();

    void onNotify(Entity& entity, Event event);
};

#endif // CLASS_SKEL_2D_MAN_H
