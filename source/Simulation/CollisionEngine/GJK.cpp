#include "GJK.h"
#include "sfVectorMath.h"
#include "stringConversion.h"

#include <cassert>

bool Simplex::contains(sf::Vector2f point)
{
    sf::Vertex a = getLast();

    sf::Vector2f AO = -a.position;

    if(vertices.size() == 3)
    {
        sf::Vertex b = vertices[0];
        sf::Vertex c = vertices[1];

        sf::Vector2f AB = b.position - a.position;
        sf::Vector2f AC = c.position - a.position;

        sf::Vector2f abPerp = sfVectorMath::tripleProduct(AC, AB, AB);
        sf::Vector2f acPerp = sfVectorMath::tripleProduct(AB, AC, AC);

        if(sfVectorMath::dot(abPerp, AO) > 0.0f)
        {
            vertices.erase(vertices.begin() + 1);
            bestDirection = abPerp;
        }
        else
        {
            if(sfVectorMath::dot(acPerp, AO) > 0.0f)
            {
                vertices.erase(vertices.begin());
                bestDirection = acPerp;
            }
            else
                return true;
        }
    }
    else
    {
        sf::Vertex b = vertices[0];
        sf::Vector2f AB = b.position - a.position;

        sf::Vector2f abPerp = sfVectorMath::tripleProduct(AB, AO, AB);

        bestDirection = abPerp;
    }
    return false;
}

void Simplex::add(sf::Vertex vert)
{
    vertices.push_back(vert);
    assert(vertices.size() <= 3);
}

sf::Vertex Simplex::getLast()
{
    return vertices[vertices.size() - 1];
}

sf::Vector2f Simplex::getNewDir(sf::Vector2f point)
{
    /*if(vertices.size() == 2)
    {
        sf::Vector2f A = vertices[vertices.size()-2].position;
        sf::Vector2f B = vertices[vertices.size()-1].position;
        sf::Vector2f O = point;

        sf::Vector2f AB = B-A;
        sf::Vector2f AO = A-O;

        sf::Vector2f newDir = sfVectorMath::tripleProduct(AB, AO, AB);

        if(sfVectorMath::square(newDir) > 0.0f)
            return sfVectorMath::norm(newDir);

        return sfVectorMath::orthogonal(AB, 1.0f);
    }
    else if(vertices.size() >= 2)
    {
        sf::Vector2f AO = point - bestEdge.v1;
        sf::Vector2f AB = bestEdge.dir;

        sf::Vector2f newDir = AO * sfVectorMath::square(AB) -
                                AB * sfVectorMath::dot(AB, AO);

        return newDir;
    }

    return {0.0f, 0.0f};*/
    return bestDirection;
}


sf::Vertex GJK::farthestPointInDir(PolyVerts &p, sf::Vector2f direction)
{
    float maxProj = -1e15;
    sf::Vertex farthestVertex;
    for(int i=0; i < (int)p.size(); ++i)
    {
        float currProj = sfVectorMath::dot(p[i].position, direction);
        if(currProj > maxProj)
        {
            maxProj = currProj;
            farthestVertex = p[i];
        }
    }

    return farthestVertex;
}

sf::Vertex GJK::support(PolyVerts &p1, PolyVerts &p2, sf::Vector2f &direction)
{
    sf::Vertex point1 = farthestPointInDir(p1, direction);
    sf::Vertex point2 = farthestPointInDir(p2, -direction);

    sf::Vertex point3;
    point3.position = point1.position - point2.position;

    return point3;
}

bool GJK::isIntersecting(Polygon* p1, Polygon* p2)
{
    sf::Vector2f direction = p1->getPosition() - p2->getPosition();

    PolyVerts p1Verts = p1->constructVerts();
    PolyVerts p2Verts = p2->constructVerts();

    Simplex simp;
    simp.add(support(p1Verts, p2Verts, direction));
    direction = -direction;

    const int MAX_ITERATIONS = 100;
    int i=0;

    while(i < MAX_ITERATIONS)
    {
        simp.add(support(p1Verts, p2Verts, direction));

        if(sfVectorMath::dot(simp.getLast().position, direction) <= 0.0f)
        {
            return false;
        }
        else
        {
            if(simp.contains({0.0f, 0.0f}))
                return true;
            else
                direction = simp.getNewDir({0.0f, 0.0f});
        }
        i++;
    }
}
