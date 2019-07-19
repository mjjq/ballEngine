#include "classJoint.h"
#include "stringConversion.h"

Joint::Joint(PhysicsObject* p1, PhysicsObject* p2)
{
    if (p1 < p2)
        {
            obj1 = p1;
            obj2 = p2;
        }
        else
        {
            obj1 = p2;
            obj2 = p1;
        }

        pwm.massInertiaPairs.push_back({obj1->getMass(), obj1->getMomentInertia()});
        pwm.massInertiaPairs.push_back({obj2->getMass(), obj2->getMomentInertia()});

        pwv.velocityPairs.push_back({obj1->getVelocity(), obj1->getRotRate()});
        pwv.velocityPairs.push_back({obj2->getVelocity(), obj2->getRotRate()});
}

void Joint::update()
{

    pwv.velocityPairs[0] = {obj1->getVelocity(), obj1->getRotRate()};
    pwv.velocityPairs[1] = {obj2->getVelocity(), obj2->getRotRate()};

}

void Joint::PreStep(float inv_dt)
{

    pwv.velocityPairs[0] = {obj1->getVelocity(), obj1->getRotRate()};
    pwv.velocityPairs[1] = {obj2->getVelocity(), obj2->getRotRate()};

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
    pwv.velocityPairs[0] = {obj1->getVelocity(), obj1->getRotRate()};
    pwv.velocityPairs[1] = {obj2->getVelocity(), obj2->getRotRate()};

    //std::cout << pwv.v1 << "\n";
    //std::cout << pwv.v2 << "\n";
        CStructs::Constraint jacobian = (Constraints::makeDistanceConstraint(*obj1,
                                   *obj2));

        Constraints::solveConstraints(pwv, jacobian, pwm, lambda);
    //std::cout << pwv.v1 << "\n";
    //std::cout << pwv.v2 << "\n\n";

    obj1->setVelocity(pwv.velocityPairs[0].v);
    obj1->setRotRate(pwv.velocityPairs[0].w);
    obj2->setVelocity(pwv.velocityPairs[1].v);
    obj2->setRotRate(pwv.velocityPairs[1].w);
}
