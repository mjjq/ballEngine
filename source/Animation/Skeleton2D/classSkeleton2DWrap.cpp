#include "classSkeleton2DWrap.h"

#include "JSONSkeletonReader.h"

#include <iostream>

Subject Skeleton2DWrap::engineNotify;

Skeleton2DWrap::Skeleton2DWrap(std::string const & filename)
{
    skeleton = JSONSkeletonReader::readFromFile(filename);
    skeleton.setAnimation("idle", AnimationSet::TransitionType::Immediate);
    engineNotify.notify(*this, Event(EventType::New_SkelObj));
    generateRenderables();
}

Skeleton2DWrap::~Skeleton2DWrap()
{
    for(auto& skin : skinRenderables)
    {
        delete skin.renderObj;
    }
    skinRenderables.clear();

    engineNotify.notify(*this, Event(EventType::Delete_SkelObj));
}

void Skeleton2DWrap::generateRenderables()
{
    std::vector<SlotData > slotData = skeleton.getSlotData();
    std::vector<SkinData > skinData = skeleton.getSkinData();

    ObjectProperties properties;

    for(int i=0; i<slotData.size(); ++i)
    {
        BoneData attachedBone = getBoneData(slotData[i].bone);

        for(int j=0; j<skinData.size(); ++j)
        {
            if(skinData[j].name == slotData[i].name)
            {
                sf::Vector2f spriteSize = skinData[j].size / 2.0f;
                properties._vertices = {
                    {{-spriteSize.x, -spriteSize.y}},
                    {{spriteSize.x, -spriteSize.y}},
                    {{spriteSize.x, spriteSize.y}},
                    {{-spriteSize.x, spriteSize.y}}
                };

                properties._position = attachedBone.position + skinData[j].offset;
                properties.type = ObjectType::Polygon;
                properties.material.diffuseID = skinData[j].name + ".png";
                //properties.material.shaderID = "phong";
                properties._zPosition = (float)i / (float)slotData.size();
                std::cout << properties._zPosition << " zPos\n";

                skinRenderables.push_back({attachedBone.name, skinData[j], new Renderable(properties)});

                std::cout << attachedBone.name << "\n";
            }
        }
    }

}

void Skeleton2DWrap::updateRenderables()
{
    for(auto& skin : skinRenderables)
    {
        BoneData bone = skeleton.getBoneData(skin.boneName);

        SkinData data = skin.data;
        Renderable* tempRend = skin.renderObj;

        float xScale = skeleton.getScale().x;

        sf::Vector2f offset = data.offset.x * bone.orientation +
                            xScale * data.offset.y * Math::orthogonal(bone.orientation, 1.0f);
        tempRend->updatePosition(bone.position + offset);

        float rotation = 180.0f/ Math::PI * atan2f(bone.orientation.y,
                                bone.orientation.x);
        if(xScale < 0.0f) rotation += 180.0f;

        tempRend->updateOrientation(rotation - xScale*data.rotation);
    }
}

void Skeleton2DWrap::animate(float deltaTime)
{
    skeleton.animate(deltaTime);
    updateRenderables();
    skelSubject.notify(*this, Event{EventType::Skel_Animate});
}

std::map<std::string, BoneData > Skeleton2DWrap::getBoneData()
{
    return skeleton.getBoneData();
}

void Skeleton2DWrap::setRootPosition(sf::Vector2f const & position)
{
    setTarget(position, "root", 0);
    skelSubject.notify(*this, Event{EventType::Skel_Animate});
}

void Skeleton2DWrap::setAnimation(std::string const & animationName)
{
    skeleton.setAnimation(animationName, AnimationSet::TransitionType::Immediate);
}

void Skeleton2DWrap::setTarget(sf::Vector2f const & target)
{
    setTarget(target, "torso", -1,false, true, Skeleton2DBone::RelativeTo::Orthogonal);
    skelSubject.notify(*this, Event{EventType::Skel_Animate});
}

void Skeleton2DWrap::setTarget(sf::Vector2f const & target,
                   std::string const & chainName,
                   int chainNode,
                   bool applyOffset,
                   bool inheritOrientation,
                   Skeleton2DBone::RelativeTo relativeTo)
{
    skeleton.externalSetTarget(target,
                               chainName,
                               chainNode,
                               applyOffset,
                               inheritOrientation,
                               relativeTo);
    updateRenderables();
}

void Skeleton2DWrap::setScale(sf::Vector2f const & scale)
{
    skeleton.setScale(scale);

    for(auto& skin : skinRenderables)
    {
        skin.renderObj->setScale(scale);
    }
}

BoneData Skeleton2DWrap::getBoneData(std::string const & boneName)
{
    return skeleton.getBoneData(boneName);
}
