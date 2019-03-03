#ifndef CLASS_ARBITER_H
#define CLASS_ARBITER_H
#include <SFML/Graphics.hpp>
#include "classPhysicsObject.h"
#include "constraintSolver.h"

struct Contact
{
	sf::Vector2f position;
	sf::Vector2f normal;
	sf::Vector2f tangent;
	sf::Vector2f rA, rB;

	CStructs::Constraint jacobianN;
	CStructs::Constraint jacobianF;

	float bias;

	float lambdaN = 0.0f;
	float lambdaT = 0.0f;

	float separation;
};

struct Arbiter
{
    enum {MAX_POINTS = 2};

	Arbiter(PhysicsObject* p1, PhysicsObject* p2);

	void update();

	void PreStep(float inv_dt);
	void ApplyImpulse();

	std::vector<Contact> contacts;
	int numContacts;

	PhysicsObject* obj1;
	PhysicsObject* obj2;

	CStructs::PairWiseVel pwv;
    CStructs::PairWiseMass pwm;
	// Combined friction
	float coefFriction;
	float coefRestitution;
};

struct ArbiterKey
{
	ArbiterKey(PhysicsObject* p1, PhysicsObject* p2)
	{
		if (p1 < p2)
		{
			obj1 = p1; obj2 = p2;
		}
		else
		{
			obj1 = p2; obj2 = p1;
		}
	}

	PhysicsObject* obj1;
	PhysicsObject* obj2;
};

inline bool operator < (const ArbiterKey& a1, const ArbiterKey& a2)
{
	if (a1.obj1 < a2.obj1)
		return true;

	if (a1.obj1 == a2.obj1 && a1.obj2 < a2.obj2)
		return true;

	return false;
}

#endif // CLASS_ARBITER_H
