#include "GJK.h"
#include "sfVectorMath.h"
#include "stringConversion.h"

#include <cassert>
#include <cmath>

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
        if(sfVectorMath::square(abPerp) <= 0.0f)
            abPerp = sfVectorMath::orthogonal(AB, 1.0f);

        bestDirection = abPerp;
    }
    return false;
}

void Simplex::add(sf::Vertex vert)
{
    vertices.push_back(vert);
    assert(vertices.size() <= 3);
}

void Simplex::add(SupportData data)
{
    sData.push_back(data);
    assert(sData.size() <= 3);
}

sf::Vertex Simplex::getLast()
{
    return vertices[vertices.size() - 1];
}

sf::Vector2f Simplex::getNewDir(sf::Vector2f point)
{
    return bestDirection;
}

sf::Vertex Simplex::getVertex(int i)
{
    return vertices[i];
}

SupportData Simplex::getSData(int i)
{
    return sData[i];
}

void Simplex::setSData(int i, SupportData c)
{
    sData[i] = c;
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

sf::Vertex GJK::support(PhysicsObject* p1, PhysicsObject* p2, sf::Vector2f &direction)
{
    sf::Vertex point1 = p1->farthestPointInDir(direction);
    sf::Vertex point2 = p2->farthestPointInDir(-direction);

    sf::Vertex point3;
    point3.position = point1.position - point2.position;

    return point3;
}


SupportData GJK::supportData(PhysicsObject* p1, PhysicsObject* p2, sf::Vector2f &direction)
{
    sf::Vertex point1 = p1->farthestPointInDir(direction);
    sf::Vertex point2 = p2->farthestPointInDir(-direction);

    sf::Vertex point3;
    point3.position = point1.position - point2.position;

    SupportData data;
    data.s1 = point1;
    data.s2 = point2;
    data.difference = point3;

    return data;
}

sf::Vertex GJK::closestPToOrigin(sf::Vector2f v1, sf::Vector2f v2)
{
    float A = sfVectorMath::square(v2 - v1);
    float B = sfVectorMath::dot(v1, v2 - v1);

    float t;

    if(A <= 0)
    {
        if(sfVectorMath::square(v1) < sfVectorMath::square(v2))
            return sf::Vertex(v1);
        else
            return sf::Vertex(v2);
    }
    else
        t = -B/A;

    if(t < 0.0f)
        t = 0.0f;
    else if(t > 1.0f)
        t = 1.0f;
    sf::Vector2f p = v1 + t*(v2-v1);

    return sf::Vertex(p);
}

bool GJK::isIntersecting(PhysicsObject* p1, PhysicsObject* p2)
{
    sf::Vector2f direction = p1->getPosition() - p2->getPosition();

    Simplex simp;
    simp.add(support(p1, p2, direction));
    direction = -direction;

    const int MAX_ITERATIONS = 100;
    int i=0;

    while(i < MAX_ITERATIONS)
    {
        simp.add(support(p1, p2, direction));

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

Edge GJK::getClosestPoints(PhysicsObject* p1, PhysicsObject* p2)
{
    sf::Vector2f direction = p1->getPosition() - p2->getPosition();

    Simplex simp;
    simp.add(supportData(p1, p2, direction));
    direction = -direction;
    simp.add(supportData(p1, p2, direction));

    direction = closestPToOrigin(simp.getSData(0).difference.position,
                                  simp.getSData(1).difference.position).position;

    const float tolerance = 0.001f;
    const int MAX_ITERATIONS = 100;
    int i=0;

    sf::Vector2f Q = {0.0f, 0.0f};

    while(i < MAX_ITERATIONS)
    {
        direction = -direction;

        if(sfVectorMath::square(direction) <= 0.0f)
        {
            Q = {0.0f, 0.0f};
            break;
        }

        SupportData c = supportData(p1, p2, direction);

        float dc = sfVectorMath::dot(c.difference.position, direction);

        float da = sfVectorMath::dot(simp.getSData(0).difference.position, direction);

        if(dc - da < tolerance)
        {
            Q = direction;
            break;
        }

        sf::Vertex p1 = closestPToOrigin(simp.getSData(0).difference.position,
                                         c.difference.position);
        sf::Vertex p2 = closestPToOrigin(c.difference.position,
                                         simp.getSData(1).difference.position);

        if(sfVectorMath::square(p1.position) < sfVectorMath::square(p2.position))
        {
            simp.setSData(1, c);
            direction = p1.position;
            Q = direction;
        }
        else
        {
            simp.setSData(0, c);
            direction = p2.position;
            Q = direction;
        }
        i++;
    }

    sf::Vector2f L = simp.getSData(1).difference.position -
                    simp.getSData(0).difference.position;
    float lambda2 = -sfVectorMath::dot(L, simp.getSData(0).difference.position) /
                        sfVectorMath::square(L);
    float lambda1 = 1.0f - lambda2;

    sf::Vertex As1 = simp.getSData(0).s1;
    sf::Vertex As2 = simp.getSData(0).s2;
    sf::Vertex Bs1 = simp.getSData(1).s1;
    sf::Vertex Bs2 = simp.getSData(1).s2;

    sf::Vector2f AClosest;
    sf::Vector2f BClosest;

    if(sfVectorMath::square(L) <= 0.0f)
    {
        AClosest = As1.position;
        BClosest = As2.position;
    }
    else
    {
        if(lambda1 >= 0.0f && lambda2 >= 0.0f)
        {
            AClosest = lambda1 * As1.position + lambda2 * Bs1.position;
            BClosest = lambda1 * As2.position + lambda2 * Bs2.position;
        }
        else if(lambda1 < 0.0f)
        {
            AClosest = Bs1.position;
            BClosest = Bs2.position;
        }
        else if(lambda2 < 0.0f)
        {
            AClosest = As1.position;
            BClosest = As2.position;
        }
    }
    //std::cout << lambda1 << " " << lambda2 << " lambda\n";

    Edge returnEdge;
    returnEdge.v1 = AClosest;
    returnEdge.v2 = BClosest;
    returnEdge.dir = BClosest - AClosest;
    returnEdge.max = BClosest;

    return returnEdge;
}
