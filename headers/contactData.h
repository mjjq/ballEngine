#ifndef CONTACT_DATA_H
#define CONTACT_DATA_H

struct Contact
{
	sf::Vector2f position;
	sf::Vector2f normal;
	sf::Vector2f tangent;
	sf::Vector2f rA, rB;

	float bias;

	float lambdaN = 0.0f;
	float lambdaT = 0.0f;

	float separation;

};

inline Contact operator - (Contact const & c)
{
    Contact newC = c;
    newC.normal = -newC.normal;
    newC.tangent = -newC.tangent;
    std::swap(newC.rA, newC.rB);

    return newC;
}

#endif // CONTACT_DATA_H
