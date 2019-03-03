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

        pwm.m1 = obj1->getMass();
        pwm.m2 = obj2->getMass();
        pwm.i1 = obj1->getMomentInertia();
        pwm.i2 = obj2->getMomentInertia();

        pwv.v1 = obj1->getVelocity();
        pwv.v2 = obj2->getVelocity();
        pwv.w1 = obj1->getRotRate();
        pwv.w2 = obj2->getRotRate();
}

void Joint::update()
{

    pwv.v1 = obj1->getVelocity();
    pwv.v2 = obj2->getVelocity();
    pwv.w1 = obj1->getRotRate();
    pwv.w2 = obj2->getRotRate();

}

void Joint::PreStep(float inv_dt)
{

    pwv.v1 = obj1->getVelocity();
    pwv.v2 = obj2->getVelocity();
    pwv.w1 = obj1->getRotRate();
    pwv.w2 = obj2->getRotRate();

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
    pwv.v1 = obj1->getVelocity();
    pwv.v2 = obj2->getVelocity();
    pwv.w1 = obj1->getRotRate();
    pwv.w2 = obj2->getRotRate();

    //std::cout << pwv.v1 << "\n";
    //std::cout << pwv.v2 << "\n";
        CStructs::Constraint jacobian = (Constraints::makeDistanceConstraint(*obj1,
                                   *obj2));

        Constraints::solveConstraints(pwv, jacobian, pwm, lambda);
    //std::cout << pwv.v1 << "\n";
    //std::cout << pwv.v2 << "\n\n";

    obj1->setVelocity(pwv.v1);
    obj1->setRotRate(pwv.w1);
    obj2->setVelocity(pwv.v2);
    obj2->setRotRate(pwv.w2);
}
