#include "classJointManager.h"

void JointManager::onNotify(Component& entity,
                            Event event,
                            Container* data)
{
    switch(event.type)
    {
        case(EventType::New_Joint) :
        {
            std::cout << "adding joint " << joints.size() << "\n";
            joints.push_back((Joint*)(&entity));
            std::cout << "added joint " << joints.size() << "\n\n";
            break;
        }
        case(EventType::Delete_Joint) :
        {
            std::cout << "delete joint\n";
            Joint* joint = (Joint*)(&entity);
            for(int i=0; i<joints.size(); ++i)
            {
                if(joints[i] == joint)
                    joints.erase(joints.begin() + i);
            }
            break;
        }
        default:
            break;
    }

}

void JointManager::preStep(float invDt)
{
    for(int i=0; i<joints.size(); ++i)
        joints[i]->PreStep(invDt);
}

void JointManager::applyImpulse()
{
    for(int i=0; i<joints.size(); ++i)
        joints[i]->ApplyImpulse();
}
