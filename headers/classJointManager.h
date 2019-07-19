#ifndef CLASS_JOINTMANAGER_H
#define CLASS_JOINTMANAGER_H

#include "Observer.h"
#include "classJoint.h"

class JointManager : public Observer
{
    std::vector<Joint* > joints;
public:
    JointManager() {Joint::engineNotify.addObserver(this);}
    ~JointManager() {joints.clear();
                    Joint::engineNotify.removeObserver(this);}

    void preStep(float invDt);
    void applyImpulse();

    void clear() {std::cout << "clearing joints\n"; for(auto joint : joints) delete joint;}

    virtual void onNotify(Component& entity, Event event, Container* data = nullptr);

};

#endif // CLASS_JOINTMANAGER_H
