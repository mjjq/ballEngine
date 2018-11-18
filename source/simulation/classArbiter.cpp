#include <iostream>

#include "../../headers/classArbiter.h"
#include "../../headers/collisionDetection.h"
#include "../../headers/constraintSolver.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"

Arbiter::Arbiter(PhysicsObject* p1, PhysicsObject* p2)
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

        contacts = Collisions::resolveCollision(obj1, obj2);
        numContacts = contacts.size();
        friction = 1.0f;

        pwm.m1 = obj1->getMass();
        pwm.m2 = obj2->getMass();
        pwm.i1 = obj1->getMomentInertia();
        pwm.i2 = obj2->getMomentInertia();

        pwv.v1 = obj1->getVelocity();
        pwv.v2 = obj2->getVelocity();
        pwv.w1 = obj1->getRotRate();
        pwv.w2 = obj2->getRotRate();
}

void Arbiter::update()
{
    contacts = Collisions::resolveCollision(obj1, obj2);
    numContacts = contacts.size();

    pwv.v1 = obj1->getVelocity();
    pwv.v2 = obj2->getVelocity();
    pwv.w1 = obj1->getRotRate();
    pwv.w2 = obj2->getRotRate();

}

void Arbiter::PreStep(float inv_dt)
{


    /*for(Contact &tempCont : contacts)
    {
        tempCont.jacobian.clear();

        tempCont.jacobian.push_back(Constraints::makeContactConstraint(*obj1,
                                   *obj2,
                                   tempCont.position,
                                   tempCont.normal,
                                   tempCont.separation*tempCont.normal*inv_dt, {0.0f, 0.0f}));


        //tempCont.biasVector.push_back()
    }*/

    pwv.v1 = obj1->getVelocity();
    pwv.v2 = obj2->getVelocity();
    pwv.w1 = obj1->getRotRate();
    pwv.w2 = obj2->getRotRate();
}

void Arbiter::ApplyImpulse()
{
    pwv.v1 = obj1->getVelocity();
    pwv.v2 = obj2->getVelocity();
    pwv.w1 = obj1->getRotRate();
    pwv.w2 = obj2->getRotRate();

    for(Contact &tempCont : contacts)
    {
        sf::Vector2f relVel = pwv.v2 - pwv.v1;

        relVel -= Collisions::orthogonal(tempCont.rA, pwv.w1) -
                    Collisions::orthogonal(tempCont.rB, pwv.w2);

        sf::Vector2f tangent = relVel - sfVectorMath::dot(relVel, tempCont.normal)*tempCont.normal;

        if(sfVectorMath::square(tangent) > 0.0f)
            tangent = sfVectorMath::norm(tangent);
        else
            tangent = sfVectorMath::orthogonal(tempCont.normal, 1.0f);

        CStructs::Constraint jacobian = (Constraints::makeContactConstraint(*obj1,
                                   *obj2,
                                   tempCont.position,
                                   tempCont.normal,
                                   tempCont.separation*tempCont.normal, relVel));

        Constraints::solveConstraints(pwv, jacobian, pwm, tempCont.lambdaN);



        relVel = pwv.v2 - pwv.v1;

        relVel -= Collisions::orthogonal(tempCont.rA, pwv.w1) -
                    Collisions::orthogonal(tempCont.rB, pwv.w2);

        tangent = relVel - sfVectorMath::dot(relVel, tempCont.normal)*tempCont.normal;

        if(sfVectorMath::square(tangent) > 0.0f)
            tangent = sfVectorMath::norm(tangent);
        else
            tangent = sfVectorMath::orthogonal(tempCont.normal, 1.0f);

        jacobian = Constraints::makeFrictionConstraint(*obj1,
                                   *obj2,
                                   tempCont.position,
                                   tangent);

        Constraints::solveConstraints(pwv, jacobian, pwm, tempCont.lambdaT);
    }

    obj1->setVelocity(pwv.v1);
    obj1->setRotRate(pwv.w1);
    obj2->setVelocity(pwv.v2);
    obj2->setRotRate(pwv.w2);
}
