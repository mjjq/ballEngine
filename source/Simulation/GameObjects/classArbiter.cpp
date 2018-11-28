#include <iostream>
#include <cmath>

#include "classArbiter.h"
#include "collisionDetection.h"
#include "constraintSolver.h"
#include "sfVectorMath.h"
#include "stringConversion.h"

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

        //if(Collisions::isAABBIntersecting(obj1, obj2))
        //{
            contacts = Collisions::resolveCollision(obj1, obj2);
            numContacts = contacts.size();
        /*}
        else
        {
            contacts = {};
            numContacts = 0;
        }*/


        coefFriction = sqrtf(obj1->getCoefFriction() * obj2->getCoefFriction());
        coefRestitution = std::max(obj1->getCoefRestitution(),
                               obj2->getCoefRestitution());

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
    //if(Collisions::isAABBIntersecting(obj1, obj2))
    //{
        contacts = Collisions::resolveCollision(obj1, obj2);
        numContacts = contacts.size();
    /*}
    else
    {
        contacts = {};
        numContacts = 0;
    }*/

    pwv.v1 = obj1->getVelocity();
    pwv.v2 = obj2->getVelocity();
    pwv.w1 = obj1->getRotRate();
    pwv.w2 = obj2->getRotRate();

}

void Arbiter::PreStep(float inv_dt)
{

    pwv.v1 = obj1->getVelocity();
    pwv.v2 = obj2->getVelocity();
    pwv.w1 = obj1->getRotRate();
    pwv.w2 = obj2->getRotRate();

    for(Contact &tempCont : contacts)
    {
        tempCont.tangent = sfVectorMath::orthogonal(tempCont.normal, 1.0f);

        sf::Vector2f relVel = pwv.v2 - pwv.v1;

        relVel -= Collisions::orthogonal(tempCont.rA, pwv.w1) -
                    Collisions::orthogonal(tempCont.rB, pwv.w2);

        float restitution = coefRestitution * sfVectorMath::dot(relVel, tempCont.normal);

        float baumGarte = tempCont.separation * inv_dt;

        tempCont.bias = 0.1f * baumGarte + restitution;
    }

}

void Arbiter::ApplyImpulse()
{
    pwv.v1 = obj1->getVelocity();
    pwv.v2 = obj2->getVelocity();
    pwv.w1 = obj1->getRotRate();
    pwv.w2 = obj2->getRotRate();

    for(Contact &tempCont : contacts)
    {
        CStructs::Constraint jacobian = (Constraints::makeContactConstraint(*obj1,
                                   *obj2,
                                   tempCont.position,
                                   tempCont.normal,
                                   tempCont.bias));

        Constraints::solveConstraints(pwv, jacobian, pwm, tempCont.lambdaN);

        jacobian = Constraints::makeFrictionConstraint(*obj1,
                                   *obj2,
                                   tempCont.position,
                                   tempCont.tangent, coefFriction*tempCont.lambdaN);

        Constraints::solveConstraints(pwv, jacobian, pwm, tempCont.lambdaT);
    }

    obj1->setVelocity(pwv.v1);
    obj1->setRotRate(pwv.w1);
    obj2->setVelocity(pwv.v2);
    obj2->setRotRate(pwv.w2);
}
