#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "../../headers/sfVectorMath.h"
#include "../../headers/forces.h"

/**
    Returns change in velocity of a gravitational force. This function must be
    called twice; once for each orthogonal co-ordinate x and y.

    @param x The current position of the particle/ball.
    @param x_0 The current position of the other particle/ball to interact with.
    @param r The distance between the current and other particle/ball.
    @param G The gravitational constant.
    @param M The mass of the OTHER particle/ball.

    @return Infinitesimal change in velocity.
*/
sf::Vector2f forces::newtonForce(sf::Vector2f vec, float M, float G)
{
    float r = sqrt(sfVectorMath::square(vec));
    float dvMag = -G*M/(r*r*r);

    return sf::Vector2f{vec.x*dvMag, vec.y*dvMag};
}

/*sf::Vector2f forces::newtonForce(sf::Vector2f vec, float M, float G)
{
    float r = sqrt(sfVectorMath::dot(vec,vec));
    float dvMag = 0.1*(pow((3/r),4)-2*pow(3/r,3));

    return sf::Vector2f{vec.x*dvMag, vec.y*dvMag};
}*/
