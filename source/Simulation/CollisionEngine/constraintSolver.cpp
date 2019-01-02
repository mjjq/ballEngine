#include <iostream>
#include <cmath>

#include "constraintSolver.h"
#include "sfVectorMath.h"
#include "stringConversion.h"

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
                                                         sf::Vector2f penetVector,
                                                         sf::Vector2f relVel)
{
    CStructs::Constraint c;
    normal = normal;
    penetVector = penetVector;

    c.c1 = -normal;
    c.cw1 = -sfVectorMath::cross(contactPoint - p1.getPosition(), normal);
    c.c2 = normal;
    c.cw2 = sfVectorMath::cross(contactPoint - p2.getPosition(), normal);
    c.lambdaMin = 0.0f;
    c.lambdaMax = 1e+15;

    float baumGarte = sfVectorMath::dot(penetVector, normal);

    c.bias = 0.1f * baumGarte;
    /*std::cout << normal << "norm\n";
    std::cout << penetVector << "pen\n";
    std::cout << baumGarte << " bgarte\n";
    std::cout << c.bias << " cbias\n";*/

    return c;
}


CStructs::Constraint Constraints::makeContactConstraint(PhysicsObject &p1,
                                                        PhysicsObject &p2,
                                                         sf::Vector2f contactPoint,
                                                         sf::Vector2f normal,
                                                         float bias)
{
    CStructs::Constraint c;

    c.c1 = -normal;
    c.cw1 = -sfVectorMath::cross(contactPoint - p1.getPosition(), normal);
    c.c2 = normal;
    c.cw2 = sfVectorMath::cross(contactPoint - p2.getPosition(), normal);
    c.lambdaMin = 0.0f;
    c.lambdaMax = 1e+15;

    c.bias = bias;

    return c;
}

CStructs::Constraint Constraints::makeFrictionConstraint(PhysicsObject &p1,
                                                         PhysicsObject &p2,
                                                         sf::Vector2f contactPoint,
                                                         sf::Vector2f tangent,
                                                         float frictionLimit)
{
    CStructs::Constraint c;
    tangent = tangent;

    c.c1 = -tangent;
    c.cw1 = -sfVectorMath::cross(contactPoint - p1.getPosition(), tangent);
    c.c2 = tangent;
    c.cw2 = sfVectorMath::cross(contactPoint - p2.getPosition(), tangent);
    c.lambdaMin = -frictionLimit;
    c.lambdaMax = -c.lambdaMin;
    c.bias = 0.0f;

    //std::cout << c.cw1 << "\n";
    //std::cout << c.cw2 << "\n\n";

    return c;
}

CStructs::Constraint Constraints::makeDistanceConstraint(PhysicsObject &p1,
                                                         PhysicsObject &p2)
{
    CStructs::Constraint c;

    sf::Vector2f relPos = p1.getPosition() - p2.getPosition();
    //float crossProd = sfVectorMath::cross(p1.getPosition(), p2.getPosition());

    c.c1 = relPos;
    c.cw1 = 0.0f;//crossProd;
    c.c2 = -relPos;
    c.cw2 = 0.0f;//-crossProd;
    c.bias = 0.0f;
    c.lambdaMax = 1e+15;
    c.lambdaMin = -1e+15;

    return c;
}


void Constraints::solveConstraints(CStructs::PairWiseVel &returnVel,
                                CStructs::Constraint &j,
                                CStructs::PairWiseMass &pwm,
                                float &lambda)
{
    float d = getDenom(j, pwm);

    float dlambda =  - (multiply(j, returnVel) + j.bias)/d;

    float l0 = lambda;
    lambda = std::max(j.lambdaMin, std::min(l0 + dlambda,
                                                  j.lambdaMax));
    dlambda = lambda - l0;

    returnVel.v1 = returnVel.v1 + dlambda * j.c1 / pwm.m1;
    returnVel.v2 = returnVel.v2 + dlambda * j.c2 / pwm.m2;
    returnVel.w1 = returnVel.w1 + dlambda * j.cw1 / pwm.i1;
    returnVel.w2 = returnVel.w2 + dlambda * j.cw2 / pwm.i2;

}
