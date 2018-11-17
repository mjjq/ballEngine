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

}

void Arbiter::update()
{
    contacts = Collisions::resolveCollision(obj1, obj2);
    numContacts = contacts.size();

}

void Arbiter::ApplyImpulse()
{
    CStructs::PairWiseVel pwv;
    pwv.v1 = obj1->getVelocity();
    pwv.v2 = obj2->getVelocity();
    pwv.w1 = obj1->getRotRate();
    pwv.w2 = obj2->getRotRate();

    CStructs::PairWiseMass pwm;
    pwm.m1 = obj1->getMass();
    pwm.m2 = obj2->getMass();
    pwm.i1 = obj1->getMomentInertia();
    pwm.i2 = obj2->getMomentInertia();

    std::vector<float > lambda;
    std::vector<CStructs::Constraint > j;

    for(Contact &tempCont : contacts)
    {
        sf::Vector2f relVel = pwv.v2 - pwv.v1;

        relVel -= Collisions::orthogonal(tempCont.rA, pwv.w1) -
                    Collisions::orthogonal(tempCont.rB, pwv.w2);

        //relVel = -relVel;
        //std::cout << contactNorm << "cnorm\n";
        //std::cout << penetVector << "penet\n";
        sf::Vector2f tangent = relVel - sfVectorMath::dot(relVel, tempCont.normal)*tempCont.normal;
        if(sfVectorMath::square(tangent) > 0.0f)
            tangent = sfVectorMath::norm(tangent);

        j.push_back(Constraints::makeContactConstraint(*obj1,
                                                       *obj2,
                                                       tempCont.position,
                                                       tempCont.normal,
                                                       tempCont.separation*tempCont.normal, relVel));

        /*j.push_back(Constraints::makeFrictionConstraint(*obj1,
                                                        *obj2,
                                                        collPoint,
                                                        tangent));
        lambda.push_back(0.0f);*/
        lambda.push_back(0.0f);
    }
    Constraints::solveConstraints(pwv, j, pwm, lambda);

    obj1->setVelocity(pwv.v1);
    obj1->setRotRate(pwv.w1);
    obj2->setVelocity(pwv.v2);
    obj2->setRotRate(pwv.w2);
}
