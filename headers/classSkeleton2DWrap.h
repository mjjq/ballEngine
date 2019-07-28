#ifndef SKEL_2D_WRAP_H
#define SKEL_2D_WRAP_H

#include "Skeleton2D.h"
#include "Observer.h"

class Skeleton2DWrap : public Component
{
    Skeleton2D skeleton;

public:
    static Subject engineNotify;
    Subject skelSubject;

    Skeleton2DWrap(std::string const & filename);
    ~Skeleton2DWrap();

    void animate(float deltaTime);
    void setRootPosition(sf::Vector2f const & position);

    void setAnimation(std::string const & animationName);

    std::map<std::string, BoneData > getBoneData();

    void setTarget(sf::Vector2f const & target);

    void setScale(sf::Vector2f const & scale);

    BoneData getBoneData(std::string const & boneName);
};

#endif // SKEL_2D_WRAP_H
