/**
    @file sfVectorMath.cpp
    Purpose: Various mathematical vector operation functions for SFML's
    sf::Vector types. Includes dot product, norm etc.

    @author mjjq
*/

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <math.h>
#include "../../headers/sfVectorMath.h"

const float PI{3.14159265359};

/**
    Get the dot product between two 2D integer vectors.

    @param vec1 The first vector.
    @param vec2 The second vector.

    @return Dot product of the two vectors.
*/
int sfVectorMath::dot(sf::Vector2i vec1, sf::Vector2i vec2)
{
    int prod = vec1.x*vec2.x + vec1.y*vec2.y;
    return prod;
}

/**
    Get the dot product between two 2D float vectors.

    @param vec1 The first vector.
    @param vec2 The second vector.

    @return Dot product of the two vectors.
*/
float sfVectorMath::dot(sf::Vector2f vec1, sf::Vector2f vec2)
{
    float prod = vec1.x*vec2.x + vec1.y*vec2.y;
    return prod;
}

float sfVectorMath::square(sf::Vector2f vec)
{
    float square = vec.x*vec.x + vec.y*vec.y;
    return square;
}

/**
    Get the unit/normlised vector of an integer vector.

    @param vec1 The vector to normalise.

    @return The unit/normalised vector.
*/
sf::Vector2f sfVectorMath::norm(sf::Vector2i vec1)
{
    sf::Vector2f normVec = static_cast<sf::Vector2f>(vec1)/static_cast<float>(pow(dot(vec1,vec1),0.5));
    return normVec;
}


/**
    Get the unit/normlised vector of a float vector.

    @param vec1 The vector to normalise.

    @return The unit/normalised vector.
*/
sf::Vector2f sfVectorMath::norm(sf::Vector2f vec1)
{
    sf::Vector2f normVec = static_cast<sf::Vector2f>(vec1)/sqrtf(dot(vec1,vec1));
    return normVec;
}


/**
    Rotate the vector clockwise by a chosen angle

    @param vec1 The vector to rotate.
    @param angleDegree The angle to rotate by in degrees.

    @return The rotated vector.
*/
sf::Vector2f sfVectorMath::rotate(sf::Vector2f vec1, float angleDegree)
{
    float vec1xprime = cosf(PI * angleDegree / 180.0f)*vec1.x - sinf(PI * angleDegree / 180.0f)*vec1.y;
    float vec1yprime = sinf(PI * angleDegree / 180.0f)*vec1.x + cosf(PI * angleDegree / 180.0f)*vec1.y;

    return sf::Vector2f{vec1xprime,vec1yprime};
}


/**
    Print the float vector, format (x, y).

    @param vec The vector to print.

    @return Void.
*/
void sfVectorMath::printVector(sf::Vector2f vec)
{
   std::cout << "(" << vec.x << ", " << vec.y << ")\n";
}


/**
    Print the integer vector, format (x, y).

    @param vec The vector to print.

    @return Void.
*/
void sfVectorMath::printVector(sf::Vector2i vec)
{
    std::cout << "(" << vec.x << ", " << vec.y << ")\n";
}

float sfVectorMath::getAverage(std::vector<float> &vec)
{
    float average = 0.0f;
    for(auto &element : vec)
        average += element;
    average = average/vec.size();

    return average;
}

float sfVectorMath::cross(sf::Vector2f v1, sf::Vector2f v2)
{
    return v1.x*v2.y - v1.y*v2.x;
}

sf::Vector2f sfVectorMath::orthogonal(sf::Vector2f v, float scalar)
{
    return {v.y*scalar, -v.x*scalar};
}
