#include "classSkeleton2DWrap.h"

#include "JSONSkeletonReader.h"

#include <iostream>

Subject Skeleton2DWrap::engineNotify;

Skeleton2DWrap::Skeleton2DWrap(std::string const & filename)
{
    skeleton = JSONSkeletonReader::readFromFile(filename);
    skeleton.setAnimation("walk", AnimationSet::TransitionType::Immediate);
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

std::vector<sf::Vector2f > Skeleton2DWrap::getJointPositions()
{
    return skeleton.getJointPositions();
}

void Skeleton2DWrap::setRootPosition(sf::Vector2f const & position)
{
    skeleton.setTarget(position, "root", 0);
    skelSubject.notify(*this, Event{EventType::Skel_Animate});
}