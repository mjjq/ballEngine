#include "classJoint.h"
#include "stringConversion.h"

Subject Joint::engineNotify;

Joint::Joint(std::vector<PhysicsObject* > _objects) : objects{_objects}
{
    engineNotify.notify(*this, Event{EventType::New_Joint});
    /*if (p1 < p2)
        {
            obj1 = p1;
            obj2 = p2;
        }
        else
        {
            obj1 = p2;
            obj2 = p1;
        }*/

        for(int i=0; i<(int)objects.size(); ++i)
        {
            pwm.massInertiaPairs.push_back({objects[i]->getMass(), objects[i]->getMomentInertia()});
            pwv.velocityPairs.push_back({objects[i]->getVelocity(), objects[i]->getRotRate()});
        }
}

void Joint::update()
{

    for(int i=0; i<(int)objects.size(); ++i)
    {
        pwv.velocityPairs[i] = {objects[i]->getVelocity(),
                                objects[i]->getRotRate()};
    }

}

void Joint::PreStep(float inv_dt)
{

    for(int i=0; i<(int)objects.size(); ++i)
    {
        pwv.velocityPairs[i] = {objects[i]->getVelocity(),
                                objects[i]->getRotRate()};
    }

    /*for(Contact &tempCont : contacts)
    {
        tempCont.tangent = sfVectorMath::orthogonal(tempCont.normal, 1.0f);

        sf::Vector2f relVel = pwv.v2 - pwv.v1;

        relVel -= Collisions::orthogonal(tempCont.rA, pwv.w1) -
                    Collisions::orthogonal(tempCont.rB, pwv.w2);

        float restitution = coefRestitution * sfVectorMath::dot(relVel, tempCont.normal);

        float baumGarte = tempCont.separation * inv_dt;

        tempCont.bias = 0.1f * baumGarte + restitution;
    }*/
    lambda = 0.0f;
}

void Joint::ApplyImpulse()
{
    for(int i=0; i<(int)objects.size(); ++i)
    {
        pwv.velocityPairs[i] = {objects[i]->getVelocity(),
                                objects[i]->getRotRate()};
    }

    //std::cout << pwv.v1 << "\n";
    //std::cout << pwv.v2 << "\n";
        CStructs::Constraint jacobian = (Constraints::makeDistanceConstraint(*objects[0],
                                   *objects[1]));

        Constraints::solveConstraints(pwv, jacobian, pwm, lambda);
    //std::cout << pwv.v1 << "\n";
    //std::cout << pwv.v2 << "\n\n";

    for(int i=0; i<(int)objects.size(); ++i)
    {
        objects[i]->setVelocity(pwv.velocityPairs[i].v);
        objects[i]->setRotRate(pwv.velocityPairs[i].w);
    }
}

void PositionJoint::ApplyImpulse()
{
    for(int i=0; i<(int)objects.size(); ++i)
    {
        pwv.velocityPairs[i] = {objects[i]->getVelocity(),
                                objects[i]->getRotRate()};
    }

    /*CStructs::Constraint jacobian = (Constraints::makePositionConstraint(objects[0]->getPosition().x,
                                    position.x, 'x'));

    Constraints::solveConstraints(pwv, jacobian, pwm, lambda);*/

    CStructs::Constraint jacobian = (Constraints::makePositionConstraint(objects[0]->getPosition().y,
                                    position.y, 'y'));

    Constraints::solveConstraints(pwv, jacobian, pwm, lambda);

    jacobian = (Constraints::makeAngularConstraint(objects[0]->getRotAngle(),
                                    0.0f));

    Constraints::solveConstraints(pwv, jacobian, pwm, lambda);

    for(int i=0; i<(int)objects.size(); ++i)
    {
        objects[i]->setVelocity(pwv.velocityPairs[i].v);
        objects[i]->setRotRate(pwv.velocityPairs[i].w);
    }
}
