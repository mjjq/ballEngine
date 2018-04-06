#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <math.h>
#include "../../headers/sfVectorMath.h"

#define PI 3.14159265359

int sfVectorMath::dot(sf::Vector2i vec1, sf::Vector2i vec2)
{
    int prod = vec1.x*vec2.x + vec1.y*vec2.y;
    return prod;
}

float sfVectorMath::dot(sf::Vector2f vec1, sf::Vector2f vec2)
{
    float prod = vec1.x*vec2.x + vec1.y*vec2.y;
    return prod;
}

sf::Vector2f sfVectorMath::norm(sf::Vector2i vec1)
{
    sf::Vector2f normVec = static_cast<sf::Vector2f>(vec1)/static_cast<float>(pow(dot(vec1,vec1),0.5));
    return normVec;
}

sf::Vector2f sfVectorMath::norm(sf::Vector2f vec1)
{
    sf::Vector2f normVec = static_cast<sf::Vector2f>(vec1)/static_cast<float>(pow(dot(vec1,vec1),0.5));
    return normVec;
}

sf::Vector2f sfVectorMath::rotate(sf::Vector2f vec1, float angleDegree)
{
    float vec1xprime = cos(PI * angleDegree / 180)*vec1.x - sin(PI * angleDegree / 180)*vec1.y;
    float vec1yprime = sin(PI * angleDegree / 180)*vec1.x + cos(PI * angleDegree / 180)*vec1.y;

    return sf::Vector2f{vec1xprime,vec1yprime};
}

void sfVectorMath::printVector(sf::Vector2f vec)
{
    std::cout << "(" << vec.x << ", " << vec.y << ")\n";
}

void sfVectorMath::printVector(sf::Vector2i vec)
{
    std::cout << "(" << vec.x << ", " << vec.y << ")\n";
}
