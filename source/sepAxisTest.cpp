#include <iostream>
#include <vector>


namespace sf
{
    struct Vector2f
    {
    public:
        float x;
        float y;
    };

    struct Vertex
    {
    public:
        Vector2f position;
    };

    struct Line
    {
    public:
        Vector2f position;
        Vector2f direction;
    };
}

namespace sfVectorMath
{
    float dot(sf::Vector2f v1, sf::Vector2f v2)
    {
        return v1.x*v2.x + v1.y*v2.y;
    }
}

sf::Vector2f operator - (sf::Vector2f v1, sf::Vector2f v2)
{
    sf::Vector2f newVec = {v1.x - v2.x, v1.y - v2.y};
    return newVec;
}


sf::Vector2f operator * (float scalar, sf::Vector2f v)
{
    sf::Vector2f newVec = {scalar*v.x, scalar*v.y};
    return newVec;
}

sf::Vector2f operator + (sf::Vector2f v1, sf::Vector2f v2)
{
    sf::Vector2f newVec = {v1.x + v2.x, v1.y + v2.y};
    return newVec;
}

sf::Vector2f operator - (sf::Vector2f v)
{
    sf::Vector2f newVec = {-v.x, -v.y};
    return newVec;
}

std::vector<sf::Line > edges_of(std::vector<sf::Vertex > &vertices)
{
    std::vector<sf::Line > edges;

    unsigned int num = vertices.size();
    for(unsigned int i=0; i<num; ++i)
    {
        sf::Line newEdge;
        newEdge.position = {0.0f, 0.0f};
        newEdge.direction = vertices[(i+1)%num].position - vertices[i].position;
        edges.push_back(newEdge);
    }

    return edges;
}

sf::Vector2f orthogonal(sf::Vector2f &v)
{
    //std::cout << -v.y << " " << v.x << "\n";
    return {v.y, -v.x};
}

std::pair<bool, sf::Vector2f > isSeparatingAxis(sf::Line &orthog,
                                                std::vector<sf::Vertex> &obj1,
                                                std::vector<sf::Vertex> &obj2)
{
    float min1 = 1e+15f;
    float max1 = -min1;
    float min2 = min1;
    float max2 = -min1;

    for(sf::Vertex &vert : obj1)
    {
        float projection = sfVectorMath::dot(vert.position, orthog.direction);

        min1 = std::min(min1, projection);
        max1 = std::max(max1, projection);
    }

    for(sf::Vertex &vert : obj2)
    {
        float projection = sfVectorMath::dot(vert.position, orthog.direction);

        min2 = std::min(min2, projection);
        max2 = std::max(max2, projection);
    }

    if(max1 >= min2 && max2 >= min1)
    {
        float d = std::min(max2 - min1, max1 - min2);
        float dOSquared = d/sfVectorMath::dot(orthog.direction, orthog.direction) + 1e-10;

        sf::Vector2f pv = dOSquared*orthog.direction;

        return std::make_pair(false, pv);
    }
    else
    {
        return std::make_pair(true, sf::Vector2f{0.0f, 0.0f});

    }
}

sf::Vector2f getCentre(std::vector<sf::Vertex> &obj)
{
    float numVerts = static_cast<float>(obj.size());
    sf::Vector2f average = {0.0f, 0.0f};

    for(sf::Vertex &vert : obj)
    {
        average = average + vert.position;
    }

    return (1.0f/numVerts) * average;
}

std::pair<bool, sf::Vector2f> sepAxisTest(std::vector<sf::Vertex> &obj1,
                                          std::vector<sf::Vertex> &obj2)
{
    std::vector<sf::Line > edges = edges_of(obj1);
    std::vector<sf::Line > edges2 = edges_of(obj2);
    edges.insert(edges.end(), edges2.begin(), edges2.end());


    std::vector<sf::Line > orthogonals;
    for(sf::Line &edge : edges)
    {
        sf::Line orthog;
        orthog.position = {0.0f, 0.0f};
        orthog.direction = orthogonal(edge.direction);
        orthogonals.push_back(orthog);
    }

    std::vector<sf::Vector2f > pushVectors;
    for(sf::Line &orthog : orthogonals)
    {
        std::pair<bool, sf::Vector2f> result = isSeparatingAxis(orthog, obj1, obj2);
        if(result.first)
            return std::make_pair(false, sf::Vector2f{0.0f, 0.0f});

        else
            pushVectors.push_back(result.second);
    }

    sf::Vector2f mpv = {1e+15, 1e+15};
    for(sf::Vector2f &pv : pushVectors)
    {
        if(sfVectorMath::dot(pv,pv) < sfVectorMath::dot(mpv, mpv))
            mpv = pv;
    }

    sf::Vector2f d = getCentre(obj1) - getCentre(obj2);

    if(sfVectorMath::dot(d, mpv) > 0)
        mpv = -mpv;

    return std::make_pair(true, mpv);
}



int main()
{
    std::vector<sf::Vertex > obj1;
    obj1.push_back({{0.0f, 0.0f}});
    obj1.push_back({{1.0f, 0.0f}});
    obj1.push_back({{1.0f, 1.0f}});
    obj1.push_back({{0.0f, 1.0f}});

    std::vector<sf::Vertex > obj2;
    obj2.push_back({{2.0f, 0.0f}});
    obj2.push_back({{3.0f, 0.0f}});
    obj2.push_back({{3.0f, 1.0f}});
    obj2.push_back({{2.0f, 1.0f}});

    std::pair<bool, sf::Vector2f> result = sepAxisTest(obj1, obj2);
    std::cout << result.first << "\n";
    std::cout << result.second.x << " " << result.second.y << "\n";
    return 0;
}
