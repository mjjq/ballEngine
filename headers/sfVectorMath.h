#ifndef SF_VECTOR_MATH_H
#define SF_VECTOR_MATH_H

namespace sfVectorMath
{
    const float PI = 3.14159265359;
    int dot(sf::Vector2i vec1, sf::Vector2i vec2);
    float dot(sf::Vector2f vec1, sf::Vector2f vec2);
    float square(sf::Vector2f vec);
    sf::Vector2f norm(sf::Vector2i vec1);
    sf::Vector2f norm(sf::Vector2f vec1);
    sf::Vector2f rotate(sf::Vector2f vec1, float angleDegree);
    void printVector(sf::Vector2f vec);
    void printVector(sf::Vector2i vec);
    float getAverage(std::vector<float> &vec);
    float cross(sf::Vector2f v1, sf::Vector2f v2);
    sf::Vector2f orthogonal(sf::Vector2f v, float scalar);
    sf::Vector2f tripleProduct(sf::Vector2f v1,
                               sf::Vector2f v2,
                               sf::Vector2f v3);
}

#endif // SF_VECTOR_MATH_H
