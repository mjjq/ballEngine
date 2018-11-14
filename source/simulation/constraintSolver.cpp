#include <iostream>
#include <cmath>

#include "../../headers/constraintSolver.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"

float Constraints::multiply(CStructs::Constraint &c,
                            CStructs::PairWiseVel &v)
{
    float result = sfVectorMath::dot(c.c1, v.v1) +
                    c.cw1 * v.w1 +
                    sfVectorMath::dot(c.c2, v.v2) +
                    c.cw2 * v.w2;
    return result;
}


float Constraints::getDenom(CStructs::Constraint &c,
                            CStructs::PairWiseMass &m)
{
    return sfVectorMath::dot(c.c1, c.c1) / m.m1 +
            sfVectorMath::dot(c.c2, c.c2) / m.m2 +
            c.cw1 * c.cw1 / m.i1 +
            c.cw2 * c.cw2 / m.i2;
}


CStructs::Constraint Constraints::makeContactConstraint(PhysicsObject &p1,
                                                        PhysicsObject &p2,
                                                         sf::Vector2f contactPoint,
                                                         sf::Vector2f normal,
                                                         sf::Vector2f penetVector)
{
    CStructs::Constraint c;
    normal = -normal;
    penetVector = penetVector;

    c.c1 = -normal;
    c.cw1 = -sfVectorMath::cross(contactPoint - p1.getPosition(), normal);
    c.c2 = normal;
    c.cw2 = sfVectorMath::cross(contactPoint - p2.getPosition(), normal);
    c.lambdaMin = 0.0f;
    c.lambdaMax = 1e+15;

    sf::Vector2f relVel = p2.getVelocity() - p1.getVelocity();
    c.bias = 0.5f * sfVectorMath::dot(relVel, normal);

    float baumGarte = sfVectorMath::dot(penetVector, normal);
    //if(baumGarte < 0.0f)
    //    c.bias += 0.1f * baumGarte;
    //std::cout << normal << "\n";
    //std::cout << penetVector << "\n";

    return c;
}

CStructs::Constraint Constraints::makeFrictionConstraint(PhysicsObject &p1,
                                                         PhysicsObject &p2,
                                                         sf::Vector2f contactPoint,
                                                         sf::Vector2f tangent)
{
    CStructs::Constraint c;

    c.c1 = -tangent;
    c.cw1 = -sfVectorMath::cross(contactPoint - p1.getPosition(), tangent);
    c.c2 = tangent;
    c.cw2 = sfVectorMath::cross(contactPoint - p2.getPosition(), tangent);
    c.lambdaMin = -1.0f*2.0f*9.8f;
    c.lambdaMax = -c.lambdaMin;

    return c;
}


void Constraints::solveConstraints(CStructs::PairWiseVel &returnVel,
                                std::vector<CStructs::Constraint> &j,
                                CStructs::PairWiseMass &pwm,
                                std::vector<float> &lambda)
{
    std::vector<float> d;

    for(int i=0; i<j.size(); ++i)
    {
        d.push_back(getDenom(j[i], pwm));
    }
    for(int i=0; i<5; ++i)
    {
        for(int i=0; i<d.size(); ++i)
        {
            float dlambda =  - (multiply(j[i], returnVel) + j[i].bias)/d[i];
//                std::cout << dlambda << " lamb\n";
            float l0 = lambda[i];
            lambda[i] = std::max(j[i].lambdaMin, std::min(l0 + dlambda,
                                                          j[i].lambdaMax));
            dlambda = lambda[i] - l0;
            //if(dlambda > 0.0f)
            returnVel.v1 = returnVel.v1 + dlambda * j[i].c1 / pwm.m1;
            returnVel.v2 = returnVel.v2 + dlambda * j[i].c2 / pwm.m2;
            returnVel.w1 = returnVel.w1 + dlambda * j[i].cw1 / pwm.i1;
            returnVel.w2 = returnVel.w2 + dlambda * j[i].cw2 / pwm.i2;
        }
    }
    //std::cout << returnVel.v1 << " v1\n";
    //std::cout << returnVel.v2 << " v2\n";

}
