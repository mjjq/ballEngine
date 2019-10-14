#ifndef CONSOLVER_H
#define CONSOLVER_H

#include <SFML/Graphics.hpp>
#include "classPhysicsObject.h"

namespace CStructs
{
    struct ConstraintPair
    {
        sf::Vector2f c;
        float cw;
    };

    struct VelocityPair
    {
        sf::Vector2f v;
        float w;
    };

    struct MassInertiaPair
    {
        float m;
        float i;
    };

    struct Constraint
    {
        std::vector<ConstraintPair > constraintPairs;
        float lambdaMin = -1e+15;
        float lambdaMax = -lambdaMin;
        float bias;
    };

    struct PairWiseVel
    {
        std::vector<VelocityPair > velocityPairs;
    };

    struct PairWiseMass
    {
        std::vector<MassInertiaPair > massInertiaPairs;
    };
}

class Constraints
{
public:
    static float multiply(CStructs::Constraint &c, CStructs::PairWiseVel &v);
    static float getDenom(CStructs::Constraint &c, CStructs::PairWiseMass &m);

    static CStructs::Constraint makeContactConstraint(PhysicsObject &p1,
                                                        PhysicsObject &p2,
                                                         sf::Vector2f contactPoint,
                                                         sf::Vector2f normal,
                                                         float bias);


    static CStructs::Constraint makeFrictionConstraint(PhysicsObject &p1,
                                                       PhysicsObject &p2,
                                                        sf::Vector2f contactPoint,
                                                        sf::Vector2f tangent,
                                                        float frictionLimit);

    static CStructs::Constraint makeDistanceConstraint(PhysicsObject &p1,
                                                       PhysicsObject &p2);

    static CStructs::Constraint makePositionConstraint(PhysicsObject &p,
                                                       sf::Vector2f const & position);

    static CStructs::Constraint makePositionConstraint(float objectPos,
                                                         float targetPos,
                                                         char direction);

    static CStructs::Constraint makeSocketConstraint(sf::Vector2f const & objectPos,
                                                       sf::Vector2f const & anchorPos,
                                                       sf::Vector2f const & initRVector,
                                                       char direction);

    static CStructs::Constraint makeAngularConstraint(float objectAngle,
                                                        float targetAngle);

    static void solveConstraints(CStructs::PairWiseVel &returnVel,
                                CStructs::Constraint &j,
                                CStructs::PairWiseMass &pwm,
                                float &lambda);
};


#endif // CONSOLVER_H
