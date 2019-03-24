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
#include "sfVectorMath.h"

const float PI{3.14159265359};

/**
    Get the dot product between two 2D integer vectors.

    @param vec1 The first vector.
    @param vec2 The second vector.

    @return Dot product of the two vectors.
*/
int sfVectorMath::dot(sf::Vector2i const & vec1, sf::Vector2i const & vec2)
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
float sfVectorMath::dot(sf::Vector2f const & vec1, sf::Vector2f const & vec2)
{
    float prod = vec1.x*vec2.x + vec1.y*vec2.y;
    return prod;
}


float sfVectorMath::dot(sf::Vector3f const & v1, sf::Vector3f const & v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}


float sfVectorMath::square(sf::Vector2f const & vec)
{
    float square = vec.x*vec.x + vec.y*vec.y;
    return square;
}


float sfVectorMath::square(sf::Vector3f const & vec)
{
    return vec.x*vec.x + vec.y*vec.y + vec.z*vec.z;
}


/**
    Get the unit/normlised vector of an integer vector.

    @param vec1 The vector to normalise.

    @return The unit/normalised vector.
*/
sf::Vector2f sfVectorMath::norm(sf::Vector2i const & vec1)
{
    sf::Vector2f normVec = static_cast<sf::Vector2f>(vec1)/static_cast<float>(pow(dot(vec1,vec1),0.5));
    return normVec;
}


/**
    Get the unit/normlised vector of a float vector.

    @param vec1 The vector to normalise.

    @return The unit/normalised vector.
*/
sf::Vector2f sfVectorMath::norm(sf::Vector2f const & vec1)
{
    return vec1/sqrtf(square(vec1));
}


sf::Vector3f sfVectorMath::norm(sf::Vector3f const & vec)
{
    return vec / sqrt(square(vec));
}


float sfVectorMath::cross(sf::Vector2f const & v1, sf::Vector2f const & v2)
{
    return v1.x*v2.y - v1.y*v2.x;
}


sf::Vector3f sfVectorMath::cross(sf::Vector3f const & v1,
                                 sf::Vector3f const & v2)
{
    float a = v1.y * v2.z - v1.z * v2.y;
    float b = v1.z * v2.x - v1.x * v2.z;
    float c = v1.x * v2.y - v1.y * v2.x;

    return sf::Vector3f(a,b,c);
}


sf::Vector2f sfVectorMath::orthogonal(sf::Vector2f const & v, float const & scalar)
{
    return {v.y*scalar, -v.x*scalar};
}


/**
    Rotate the vector clockwise by a chosen angle

    @param vec1 The vector to rotate.
    @param angleDegree The angle to rotate by in degrees.

    @return The rotated vector.
*/
sf::Vector2f sfVectorMath::rotate(sf::Vector2f const & vec1, float const & angleDegree)
{
    float cosine = cosf(PI * angleDegree / 180.0f);
    float sine = sinf(PI * angleDegree / 180.0f);

    float vec1xprime = cosine*vec1.x - sine*vec1.y;
    float vec1yprime = sine*vec1.x + cosine*vec1.y;

    return sf::Vector2f{vec1xprime,vec1yprime};
}


sf::Vector2f sfVectorMath::tripleProduct(sf::Vector2f const & v1,
                                         sf::Vector2f const & v2,
                                         sf::Vector2f const & v3)
{
    sf::Vector2f result = v2 * sfVectorMath::dot(v3, v1) -
                        v1 * sfVectorMath::dot(v2, v3);
    return result;
}


sf::Vector2f sfVectorMath::average(std::vector<sf::Vertex > const & verts)
{
    sf::Vector2f average = {0.0f, 0.0f};
    if(verts.size() > 0)
    {
        for(int i=0; i<(int)verts.size(); ++i)
            average += verts[i].position;
        average = average/(float)verts.size();
    }

    return average;
}


float sfVectorMath::average(std::vector<float> const & vec)
{
    float average = 0.0f;
    for(auto &element : vec)
        average += element;
    average = average/vec.size();

    return average;
}


/**
    Print the float vector, format (x, y).

    @param vec The vector to print.

    @return Void.
*/
void sfVectorMath::printVector(sf::Vector2f const & vec)
{
   std::cout << "(" << vec.x << ", " << vec.y << ")\n";
}


/**
    Print the integer vector, format (x, y).

    @param vec The vector to print.

    @return Void.
*/
void sfVectorMath::printVector(sf::Vector2i const & vec)
{
    std::cout << "(" << vec.x << ", " << vec.y << ")\n";
}


int sfVectorMath::modulo(int const & n1, int const & n2)
{
    return (n2 + (n1%n2)) % n2;
}
