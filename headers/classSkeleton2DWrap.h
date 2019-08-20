#ifndef SKEL_2D_WRAP_H
#define SKEL_2D_WRAP_H

#include "Skeleton2D.h"
#include "Observer.h"
#include "classRenderable.h"

struct SkinDataRenderable
{
    std::string boneName;
    SkinData data;
    Renderable* renderObj;
};

class Skeleton2DWrap : public Component
{
    Skeleton2D skeleton;
    std::vector<SkinDataRenderable > skinRenderables;

    void generateRenderables();
    void updateRenderables();
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

    void setTarget(sf::Vector2f const & target,
                   std::string const & chainName,
                   int chainNode,
                   bool applyOffset = false,
                   bool inheritOrientation = true,
                   Skeleton2DBone::RelativeTo relativeTo = Skeleton2DBone::RelativeTo::World);

    void setScale(sf::Vector2f const & scale);

    BoneData getBoneData(std::string const & boneName);
};

#endif // SKEL_2D_WRAP_H
