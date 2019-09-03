#include <iostream>
#include <cmath>

#include "classArbiter.h"
#include "collisionDetection.h"
#include "constraintSolver.h"
#include "Math.h"
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
            contacts = Collisions::findCollisions(obj1, obj2);
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

        /*pwm.m1 = obj1->getMass();
        pwm.m2 = obj2->getMass();
        pwm.i1 = obj1->getMomentInertia();
        pwm.i2 = obj2->getMomentInertia();

        pwv.v1 = obj1->getVelocity();
        pwv.v2 = obj2->getVelocity();
        pwv.w1 = obj1->getRotRate();
        pwv.w2 = obj2->getRotRate();*/

        pwm.massInertiaPairs.push_back({obj1->getMass(), obj1->getMomentInertia()});
        pwm.massInertiaPairs.push_back({obj2->getMass(), obj2->getMomentInertia()});

        pwv.velocityPairs.push_back({obj1->getVelocity(), obj1->getRotRate()});
        pwv.velocityPairs.push_back({obj2->getVelocity(), obj2->getRotRate()});
}

void Arbiter::update()
{
    //if(Collisions::isAABBIntersecting(obj1, obj2))
    //{
        contacts = Collisions::findCollisions(obj1, obj2);
        numContacts = contacts.size();

        coefFriction = sqrtf(obj1->getCoefFriction() * obj2->getCoefFriction());
        coefRestitution = std::max(obj1->getCoefRestitution(),
                               obj2->getCoefRestitution());
    /*}
    else
    {
        contacts = {};
        numContacts = 0;
    }*/

    pwv.velocityPairs[0] = {obj1->getVelocity(), obj1->getRotRate()};
    pwv.velocityPairs[1] = {obj2->getVelocity(), obj2->getRotRate()};

}

void Arbiter::PreStep(float inv_dt)
{

    pwv.velocityPairs[0] = {obj1->getVelocity(), obj1->getRotRate()};
    pwv.velocityPairs[1] = {obj2->getVelocity(), obj2->getRotRate()};

    for(Contact &tempCont : contacts)
    {
        tempCont.tangent = Math::orthogonal(tempCont.normal, 1.0f);

        sf::Vector2f v1 = pwv.velocityPairs[0].v;
        sf::Vector2f v2 = pwv.velocityPairs[1].v;
        float w1 = pwv.velocityPairs[0].w;
        float w2 = pwv.velocityPairs[1].w;

        sf::Vector2f relVel = v2 - v1;

        relVel -= Collisions::orthogonal(tempCont.rA, w1) -
                    Collisions::orthogonal(tempCont.rB, w2);

        float restitution = coefRestitution * Math::dot(relVel, tempCont.normal);

        float baumGarte = tempCont.separation * inv_dt;

        tempCont.bias = 0.1f * baumGarte + restitution;
    }

}

void Arbiter::ApplyImpulse()
{
    pwv.velocityPairs[0] = {obj1->getVelocity(), obj1->getRotRate()};
    pwv.velocityPairs[1] = {obj2->getVelocity(), obj2->getRotRate()};

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

    obj1->setVelocity(pwv.velocityPairs[0].v);
    obj1->setRotRate(pwv.velocityPairs[0].w);
    obj2->setVelocity(pwv.velocityPairs[1].v);
    obj2->setRotRate(pwv.velocityPairs[1].w);
}
