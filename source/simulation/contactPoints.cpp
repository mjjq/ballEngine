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

std::vector<sf::Vector2f > Collisions::getContactPoints(std::vector<sf::Vertex > &obj1,
                                                        std::vector<sf::Vertex > &obj2,
                                                        sf::Vector2f contactNormal)
{
    //contactNormal = -contactNormal;
    Edge edge1 = Collisions::getBestEdge(obj1, -contactNormal);
    Edge edge2 = Collisions::getBestEdge(obj2, contactNormal);

    //std::cout << contactNormal << "\n";

    sf::Vertex line1[] = {
        sf::Vertex(edge1.v1),
        sf::Vertex(edge1.v2)
    };
    sf::Vertex line2[] = {
        sf::Vertex(edge2.v1),
        sf::Vertex(edge2.v2)
    };
    debugWindow->draw(line1, 2, sf::Lines);
    debugWindow->draw(line2, 2, sf::Lines);

    bool flip = false;
    Edge refEdge;
    Edge incEdge;
    /*if(std::abs(sfVectorMath::dot(edge1.dir, contactNormal)) <=
       std::abs(sfVectorMath::dot(edge2.dir, contactNormal)))
    {
        refEdge = edge1;
        incEdge = edge2;
    }
    else
    {
        refEdge = edge2;
        incEdge = edge1;

        flip = true;
    }*/

    refEdge = edge2;
    incEdge = edge1;
    flip = true;
    //std::cout << refEdge.v1 << " v1\n";
   // std::cout << refEdge.v2 << " v2\n";


    sf::Vector2f refDir = sfVectorMath::norm(refEdge.dir);
    //std::cout << refDir <<" diredtion\n";

    float o1 = sfVectorMath::dot(refDir, refEdge.v1);

    ClippedPoints cp = Collisions::clip(incEdge.v1, incEdge.v2, refDir, o1);
    if(cp.size() < 2)
        return ClippedPoints{};

        //std::cout << refDir << " refdir\n\n";
    float o2 = sfVectorMath::dot(refDir, refEdge.v2);
    cp = clip(cp[0], cp[1], -refDir, -o2);



    if(cp.size() < 2)
        return ClippedPoints{};

    sf::Vector2f refNorm = Collisions::orthogonal(refDir, -1.0f);


    if(flip) refNorm = -refNorm;

    /*std::cout << refNorm << "\n";
    std::cout << refEdge.v1 << "\n";*/

    float maxD = sfVectorMath::dot(refNorm, refEdge.max);

    /*std::cout << maxD << " maxD\n";
    std::cout << cp[0] << " " << cp[1] << "\n";*/

    ClippedPoints cpFinal;

    if(sfVectorMath::dot(refNorm, cp[0]) - maxD <= 0.0f)
    {
        cpFinal.push_back(cp[0]);
        //std::cout << "cp0 true\n";
    }
    if(sfVectorMath::dot(refNorm, cp[1]) - maxD <= 0.0f)
    {
        cpFinal.push_back(cp[1]);
        //std::cout << "cp1 true\n";
    }

    //std::cout << cpFinal[0] << " " << cpFinal[1] << "\n";
    //std::cout << cp.size() << "\n";
if(cpFinal.size() >= 1)
    {
    sf::CircleShape circ1{2.5f};
    circ1.setPosition(cpFinal[0]);
    circ1.setOrigin({1.25f, 1.25f});
    if(cpFinal.size() >= 2)
    {
    sf::CircleShape circ2{2.5f};
    circ2.setOrigin({1.25f, 1.25f});
    circ2.setPosition(cpFinal[1]);

    debugWindow->draw(circ2);
    }
    debugWindow->draw(circ1);

    }

    return cpFinal;
}


std::vector<sf::Vector2f > Collisions::getContactPoints(std::vector<sf::Vertex > &obj1,
                                                        Ball &obj2,
                                                        sf::Vector2f contactNormal)
{
    Edge edge1 = Collisions::getBestEdge(obj1, contactNormal);
    //std::cout << contactNormal << "norm\n";
    //std::cout << edge1.dir << "edge\n";

    sf::Vertex line1[] = {
        sf::Vertex(edge1.v1),
        sf::Vertex(edge1.v2)
    };
    debugWindow->draw(line1, 2, sf::Lines);

    bool flip = false;
    Edge refEdge;

    refEdge = edge1;
    flip = true;
    //std::cout << refEdge.v1 << " v1\n";
   // std::cout << refEdge.v2 << " v2\n";


    sf::Vector2f refDir = sfVectorMath::norm(refEdge.dir);
    //std::cout << refDir <<" diredtion\n";

    float o1 = 1.0f*sfVectorMath::dot(refDir, refEdge.v1);

    ClippedPoints cp = Collisions::getIntPoint(obj2, refEdge);

 /*if(cp.size() >= 1)
    {
    sf::CircleShape circ1{2.5f};
    circ1.setPosition(cp[0]);
    circ1.setOrigin({1.25f, 1.25f});
    std::cout << refEdge.v1 << "v1\n";
    std::cout << refEdge.v2 << "v2\n";
    std::cout << refDir << "v2\n";
    std::cout << cp[0] << "\n";
    if(cp.size() >= 2)
    {
    sf::CircleShape circ2{2.5f};
    circ2.setOrigin({1.25f, 1.25f});
    circ2.setPosition(cp[1]);
    std::cout << cp[1] << "\n";
    debugWindow->draw(circ2);
    }
    debugWindow->draw(circ1);

    }*/

    if(cp.size() < 2)
    {
        return cp;
    }

    cp = clip(cp[0], cp[1], refDir, o1);



    //std::cout << cp.size();

    if(cp.size() < 2)
    {
        return cp;
    }

        //std::cout << refDir << " refdir\n\n";
    float o2 = 1.0f*sfVectorMath::dot(refDir, refEdge.v2);
    cp = clip(cp[0], cp[1], -refDir, -o2);




    return cp;
}


ClippedPoints Collisions::getIntPoint(Ball &ball, Edge &edge)
{
    sf::Vector2f relR = edge.v1 - ball.getPosition();// -sf::Vector2f{ball.getRadius(), ball.getRadius()};
    float A = sfVectorMath::square(edge.dir);
    float B = 2.0f*sfVectorMath::dot(edge.dir, relR);
    float C = sfVectorMath::square(relR) -
                ball.getRadius()*ball.getRadius();

    float discriminant = B*B - 4*A*C;

   /* std::cout << B*B << "\n";
    std::cout << 4*A*C << "\n";
    std::cout << discriminant << "\n";*/
/*
    sf::CircleShape circ1{50.0f};
    circ1.setPosition(ball.getPosition() - sf::Vector2f{50.0f, 50.0f});
    circ1.setOrigin({1.25f, 1.25f});
    circ1.setFillColor(sf::Color::Red);
    debugWindow->draw(circ1);*/


    if(discriminant < 1e-5)
    {
        //std::cout << "single sol\n";
        float t = -B/(2*A);
        sf::Vector2f cp0 = edge.v1 + edge.dir * t;
        ClippedPoints cp;
        cp.push_back(cp0);
        //std::cout << cp[0] << " cp\n";
        return cp;
    }

    float t0 = (-B + 1.0f*sqrt(discriminant))/(2*A);
    float t1 = (-B - 1.0f*sqrt(discriminant))/(2*A);



    sf::Vector2f cp0 = edge.v1 + edge.dir * t0;
    sf::Vector2f cp1 = edge.v1 + edge.dir * t1;

    ClippedPoints cp;

    cp.push_back(cp0);
    cp.push_back(cp1);






    return cp;
}


Edge Collisions::getBestEdge(std::vector<sf::Vertex > &obj, sf::Vector2f normal)
{

    int index = 0;
    float max = -1e+15;

    int c = obj.size();
    for (int i = 0; i < c; ++i)
    {
      float projection = sfVectorMath::dot(normal, obj[i].position);
      if (projection > max)
      {
        max = projection;
        index = i;
      }
    }

    //std::cout << obj[0].position << "\n";
    //std::cout << "normal " << normal << "\n\n";
    sf::Vector2f v = obj[index%c].position;
    sf::Vector2f v1 = obj[(index+1)%c].position;
    sf::Vector2f v0;
    if(index == 0)
        v0 = obj[c-1].position;
    else
        v0 = obj[index-1].position;

    sf::Vector2f l = sfVectorMath::norm(v - v1);
    sf::Vector2f r = sfVectorMath::norm(v - v0);

    /*(std::cout << normal << " " << index << "\n";
    std::cout << obj[index].position << "\n";
    std::cout << "l: " << l << "\n";
    std::cout << "r: " << r << "\n\n";*/
    /*sf::CircleShape circ1{2.5f};
    circ1.setPosition(v);
    circ1.setOrigin({1.25f, 1.25f});
    circ1.setFillColor(sf::Color::Red);
    sf::CircleShape circ2{2.5f};
    circ2.setPosition(v1);
    circ2.setOrigin({1.25f, 1.25f});
    circ2.setFillColor(sf::Color::Green);
    sf::CircleShape circ3{2.5f};
    circ3.setPosition(v0);
    circ3.setOrigin({1.25f, 1.25f});
    debugWindow->draw(circ1);
    debugWindow->draw(circ2);
    debugWindow->draw(circ3);*/


    if(sfVectorMath::dot(r, normal) <= sfVectorMath::dot(l, normal))
    {
        return Edge{v0, v, v-v0, v};
    }
    else
        return Edge{v, v1, v1-v, v};

}


ClippedPoints Collisions::clip(sf::Vector2f v1,
                               sf::Vector2f v2,
                               sf::Vector2f normal,
                               float o)
{
    ClippedPoints cp;

    float d1 = sfVectorMath::dot(normal, v1) - o;
    float d2 = sfVectorMath::dot(normal, v2) - o;

    if(d1 >= 0.0f) cp.push_back(v1);
    if(d2 >= 0.0f) cp.push_back(v2);

    //std::cout << d1 << " d1:d2 " << d2 << "\n";
    //std::cout << v1 << " " << v2 << "\n";
    //std::cout << normal << " " << o << "\n\n";

    if(d1*d2 < 0.0f)
    {
        sf::Vector2f e = v2 - v1;
        float u = d1 / (d1-d2);

        e = u*e;
        e += v1;
        cp.push_back(e);
       // std::cout << cp[0] << " " << cp[1] << "\n\n";
    }

    //std::cout << cp.size() << "newsize\n";
    return cp;
}