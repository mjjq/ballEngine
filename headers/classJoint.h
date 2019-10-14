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
    virtual ~Joint() { engineNotify.notify(*this, Event{EventType::Delete_Joint}); }

	Joint(std::vector<PhysicsObject* > _objects);

	void update();

	void PreStep(float inv_dt);
	virtual void ApplyImpulse();

	bool containsValidObject();
};

class PositionJoint : public Joint
{
    std::function<sf::Vector2f() > getPosition;
    std::function<float() > getRotation;
public:
    PositionJoint(std::vector<PhysicsObject * > _objects,
                  std::function<sf::Vector2f() > _getPosition,
                  std::function<float() > _getRotation) :
                    Joint{_objects}, getPosition{_getPosition},
                    getRotation{_getRotation} {}

    virtual void ApplyImpulse();
};

class SocketJointSingle : public Joint
{
    std::function<sf::Vector2f() > getAnchorPosition;
    float initialRotation = 0.0f;
    sf::Vector2f localVirtualAnchor;

public:
    SocketJointSingle(std::vector<PhysicsObject * > _objects,
                      std::function<sf::Vector2f() > _getAnchorPosition) :
                      Joint{_objects}, getAnchorPosition{_getAnchorPosition}
  {
        sf::Vector2f objPos = _objects[0]->getPosition();
        float objRot = _objects[0]->getRotAngle();

        sf::Vector2f relativePosition = getAnchorPosition() - objPos;
        localVirtualAnchor = relativePosition;
        initialRotation = objRot;
  }

    virtual void ApplyImpulse();
};

#endif // CLASS_JOINT_H
