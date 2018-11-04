#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <cassert>

#include "../../headers/collisionDetection.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"

sf::Rect<float > Collisions::minkowskiDiff(AABB &rect1, AABB &rect2)
{
    sf::Rect<float > rectBounds1 = rect1.getGlobalBounds();
    sf::Rect<float > rectBounds2 = rect2.getGlobalBounds();

    sf::Vector2f topLeft = {rectBounds1.left - (rectBounds2.left + rectBounds2.width),
                            rectBounds1.top - (rectBounds2.top + rectBounds2.height)};

    sf::Vector2f topRight = topLeft + sf::Vector2f{rectBounds1.width + rectBounds2.width, 0.0f};

    sf::Vector2f bottomLeft = topLeft + sf::Vector2f{0.0f, rectBounds1.height + rectBounds2.height};

    sf::Rect<float > result{topLeft.x, topLeft.y,
                            topRight.x - topLeft.x,
                            bottomLeft.y - topLeft.y};

    return result;
}

std::vector<sf::Vertex > Collisions::minkowskiDiff(std::vector<sf::Vertex > &obj1,
                                                   std::vector<sf::Vertex > &obj2)
{
    std::vector<sf::Vertex > pureDifference;

    for(sf::Vertex &vertObj1 : obj1)
        for(sf::Vertex &vertObj2 : obj2)
        {
            pureDifference.push_back(sf::Vertex(sf::Vector2f(vertObj1.position - vertObj2.position)));
        }

    return pureDifference;
}
