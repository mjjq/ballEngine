#include "classSkeleton2DWrap.h"

#include "JSONSkeletonReader.h"

#include <iostream>

Subject Skeleton2DWrap::engineNotify;

Skeleton2DWrap::Skeleton2DWrap(std::string const & filename)
{
    skeleton = JSONSkeletonReader::readFromFile(filename);
    skeleton.setAnimation("idle", AnimationSet::TransitionType::Immediate);
    engineNotify.notify(*this, Event(EventType::New_SkelObj));
}

Skeleton2DWrap::~Skeleton2DWrap()
{
    engineNotify.notify(*this, Event(EventType::Delete_SkelObj));
}

void Skeleton2DWrap::animate(float deltaTime)
{
    skeleton.animate(deltaTime);
    skelSubject.notify(*this, Event{EventType::Skel_Animate});
}

std::map<std::string, BoneData > Skeleton2DWrap::getBoneData()
{
    return skeleton.getBoneData();
}

void Skeleton2DWrap::setRootPosition(sf::Vector2f const & position)
{
    skeleton.externalSetTarget(position, "root", 0);
    skelSubject.notify(*this, Event{EventType::Skel_Animate});
}

void Skeleton2DWrap::setAnimation(std::string const & animationName)
{
    skeleton.setAnimation(animationName, AnimationSet::TransitionType::Immediate);
}

void Skeleton2DWrap::setTarget(sf::Vector2f const & target)
{
    skeleton.externalSetTarget(target, "torso", -1,false, true, Skeleton2DBone::RelativeTo::Orthogonal);
    skelSubject.notify(*this, Event{EventType::Skel_Animate});
}

void Skeleton2DWrap::setScale(sf::Vector2f const & scale)
{
    skeleton.setScale(scale);
}

BoneData Skeleton2DWrap::getBoneData(std::string const & boneName)
{
    return skeleton.getBoneData(boneName);
}
