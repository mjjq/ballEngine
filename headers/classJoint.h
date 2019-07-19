#ifndef CLASS_JOINT_H
#define CLASS_JOINT_H

#include <SFML/Graphics.hpp>
#include "classPhysicsObject.h"
#include "constraintSolver.h"
#include "Observer.h"

class Joint : public Component
{
protected:
	float lambda = 0.0f;

	std::vector<PhysicsObject* > objects;

	CStructs::PairWiseVel pwv;
    CStructs::PairWiseMass pwm;
    enum {MAX_POINTS = 2};

public:
    static Subject engineNotify;
    Subject jointSubject;

    Joint() { engineNotify.notify(*this, Event{EventType::New_Joint}); }
    ~Joint() { engineNotify.notify(*this, Event{EventType::Delete_Joint}); }

	Joint(std::vector<PhysicsObject* > _objects);

	void update();

	void PreStep(float inv_dt);
	virtual void ApplyImpulse();
};

class PositionJoint : public Joint
{
    sf::Vector2f position;
public:
    PositionJoint(std::vector<PhysicsObject * > _objects,
                  sf::Vector2f const & _position) :
                    Joint{_objects}, position{_position} {}

    virtual void ApplyImpulse();
};

#endif // CLASS_JOINT_H
