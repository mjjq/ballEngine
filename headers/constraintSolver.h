#ifndef CONSOLVER_H
#define CONSOLVER_H

#include <SFML/Graphics.hpp>
#include "classPhysicsObject.h"

namespace CStructs
{
    struct Constraint
    {
        sf::Vector2f c1;
        float cw1;
        sf::Vector2f c2;
        float cw2;
        float lambdaMin;
        float lambdaMax;
        float bias;
    };

    struct PairWiseVel
    {
        sf::Vector2f v1;
        float w1;
        sf::Vector2f v2;
        float w2;
    };

    struct PairWiseMass
    {
        float m1;
        float i1;
        float m2;
        float i2;
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
                                                     sf::Vector2f penetVector,
                                                     sf::Vector2f relVel);

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



    static void solveConstraints(CStructs::PairWiseVel &returnVel,
                                CStructs::Constraint &j,
                                CStructs::PairWiseMass &pwm,
                                float &lambda);
};


#endif // CONSOLVER_H
