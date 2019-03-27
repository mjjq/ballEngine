#ifndef CLASS_JOINT_H
#define CLASS_JOINT_H

#include <SFML/Graphics.hpp>
#include "classPhysicsObject.h"
#include "constraintSolver.h"

struct Joint
{
    enum {MAX_POINTS = 2};

	Joint(PhysicsObject* p1, PhysicsObject* p2);

	void update();

	void PreStep(float inv_dt);
	void ApplyImpulse();

	float lambda = 0.0f;

	PhysicsObject* obj1;
	PhysicsObject* obj2;

	CStructs::PairWiseVel pwv;
    CStructs::PairWiseMass pwm;
};

#endif // CLASS_JOINT_H
