#include <iostream>
#include <cmath>

#include "constraintSolver.h"
#include "Math.h"
#include "stringConversion.h"

float Constraints::multiply(CStructs::Constraint &c,
                            CStructs::PairWiseVel &v)
{
    float result = 0.0f;
    for(int i=0; i<(int)c.constraintPairs.size(); ++i)
    {
        result += Math::dot(c.constraintPairs[i].c, v.velocityPairs[i].v) +
                    c.constraintPairs[i].cw * v.velocityPairs[i].w;
    }

    return result;
}


float Constraints::getDenom(CStructs::Constraint &c,
                            CStructs::PairWiseMass &m)
{
    float result = 0.0f;
    for(int i=0; i<(int)c.constraintPairs.size(); ++i)
    {
        result += Math::square(c.constraintPairs[i].c) /
                    m.massInertiaPairs[i].m +
                    pow(c.constraintPairs[i].cw, 2) / m.massInertiaPairs[i].i;
    }

    return result;
}



CStructs::Constraint Constraints::makeContactConstraint(PhysicsObject &p1,
                                                        PhysicsObject &p2,
                                                         sf::Vector2f contactPoint,
                                                         sf::Vector2f normal,
                                                         float bias)
{
    CStructs::Constraint c;

    c.constraintPairs.push_back({-normal, -Math::cross(contactPoint - p1.getPosition(), normal)});
    c.constraintPairs.push_back({normal, Math::cross(contactPoint - p2.getPosition(), normal)});

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

    c.constraintPairs.push_back({-tangent, -Math::cross(contactPoint - p1.getPosition(), tangent)});
    c.constraintPairs.push_back({tangent, Math::cross(contactPoint - p2.getPosition(), tangent)});
    c.lambdaMin = -frictionLimit;
    c.lambdaMax = -c.lambdaMin;
    c.bias = 0.0f;

    return c;
}

CStructs::Constraint Constraints::makeDistanceConstraint(PhysicsObject &p1,
                                                         PhysicsObject &p2)
{
    CStructs::Constraint c;

    sf::Vector2f relPos = p1.getPosition() - p2.getPosition();

    c.constraintPairs.push_back({relPos, 0.0f});
    c.constraintPairs.push_back({-relPos, 0.0f});

    c.bias = 0.0f;
    c.lambdaMax = 1e+15;
    c.lambdaMin = -1e+15;

    return c;
}

CStructs::Constraint Constraints::makePositionConstraint(PhysicsObject &p,
                                                         sf::Vector2f const & position)
{
    CStructs::Constraint c;

    sf::Vector2f relPos = p.getPosition() - position;

    c.constraintPairs.push_back({relPos, 0.0f});

    c.bias = 0.05f*(Math::square(relPos));

    std::cout << c.bias << " bias\n";
    c.lambdaMax = 1000.0f;
    c.lambdaMin = -1000.0f;

    return c;
}

CStructs::Constraint Constraints::makePositionConstraint(float objectPos,
                                                         float targetPos,
                                                         char direction)
{
    CStructs::Constraint c;

    if(direction == 'x')
    {
        c.constraintPairs.push_back({{1.0f, 0.0}, 0.0f});
    }
    else if(direction == 'y')
    {
        c.constraintPairs.push_back({{0.0f, 1.0}, 0.0f});
    }


    c.bias = 0.5f*(objectPos - targetPos);

    c.lambdaMax = 1000.0f;
    c.lambdaMin = -1000.0f;

    return c;
}

CStructs::Constraint Constraints::makeSocketConstraint(sf::Vector2f const & objectPos,
                                                       sf::Vector2f const & anchorPos,
                                                       sf::Vector2f const & initRVector,
                                                       char direction)
{
    CStructs::Constraint c;

    sf::Vector2f rVector = anchorPos - objectPos;

    sf::Vector2f biasVector = 0.5f*(objectPos + initRVector - anchorPos);

    if(direction == 'x')
    {
        c.constraintPairs.push_back({{1.0f, 0.0}, -rVector.y});
        c.bias = biasVector.x;
    }
    else if(direction == 'y')
    {
        c.constraintPairs.push_back({{0.0f, 1.0}, rVector.x});
        c.bias = biasVector.y;
    }

    c.lambdaMax = 1000.0f;
    c.lambdaMin = -1000.0f;

    return c;
}

CStructs::Constraint Constraints::makeAngularConstraint(float objectAngle,
                                                        float targetAngle)
{
    CStructs::Constraint c;

    c.constraintPairs.push_back({{0.0f, 0.0}, 1.0f});

    c.bias = 0.01f*(objectAngle - targetAngle);

    c.lambdaMax = 1000.0f;
    c.lambdaMin = -1000.0f;

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

    for(int i=0; i<returnVel.velocityPairs.size(); ++i)
    {
        returnVel.velocityPairs[i].v += dlambda * j.constraintPairs[i].c / pwm.massInertiaPairs[i].m;
        returnVel.velocityPairs[i].w += dlambda * j.constraintPairs[i].cw / pwm.massInertiaPairs[i].i;
    }

}
