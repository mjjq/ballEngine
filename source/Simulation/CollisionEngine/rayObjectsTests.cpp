#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <cassert>

#include "collisionDetection.h"
#include "Math.h"
#include "stringConversion.h"

float Collisions::rayAABBIntersect(sf::Vector2f rayStart,
                                  sf::Vector2f rayDir,
                                  sf::Rect<float > &rectAABB,
                                  float tmin, float tmax, float epsilon)
{
    sf::Vector2f r = rayStart;
    sf::Vector2f v = rayDir;

    /*if(AABB.contains(r.x,r.y))
    {
        //colliderArray.printMatrix(); std::cout << "\n";
        //staticCollArray.printMatrix(); std::cout << "\n";
        return std::numeric_limits<float>::quiet_NaN();
    }*/

    if(std::abs(v.x) < epsilon)
    {
        if(r.x < rectAABB.left || r.x > rectAABB.left + rectAABB.width)
            return std::numeric_limits<float>::quiet_NaN();
    }
    else
    {
        float ood = 1.0f / v.x;
        float t1 = (rectAABB.left - r.x) * ood;
        float t2 = (rectAABB.left + rectAABB.width - r.x) * ood;

        if(t1>t2) std::swap(t1,t2);
        tmin = std::max(t1, tmin);
        tmax = std::min(t2, tmax);
        if(tmin > tmax) return std::numeric_limits<float>::quiet_NaN();
    }

    if(std::abs(v.y) < epsilon)
    {
        if(r.y < rectAABB.top || r.y > rectAABB.top + rectAABB.height)
            return std::numeric_limits<float>::quiet_NaN();
    }
    else
    {
        float ood = 1.0f / v.y;
        float t1 = (rectAABB.top - r.y) * ood;
        float t2 = (rectAABB.top + rectAABB.height - r.y) * ood;

        if(t1>t2) std::swap(t1,t2);
        tmin = std::max(t1, tmin);
        tmax = std::min(t2, tmax);
        if(tmin > tmax) return std::numeric_limits<float>::quiet_NaN();
    }
    //if(tmin < 0.0f || tmax < 0.0f)
    //    tmin = 1e+12;
    //std::cout << tmin << "\n";
    return tmin;
}

float Collisions::raySphereIntersect(sf::Vector2f rayOrigin, sf::Vector2f rayDir,
                                     sf::Vector2f sphereCentre, float sphereRadius)
{
    float a = Math::square(rayDir);
    float b = 2.0f*Math::dot(rayDir, (rayOrigin-sphereCentre) );
    float c = Math::square( rayOrigin-sphereCentre )
                                                - sphereRadius*sphereRadius;
    float discriminant = b*b - 4.0f*a*c;
    if(discriminant < 0)
        return std::numeric_limits<float>::quiet_NaN();

    //std::cout << rayOrigin << "\n";
    //std::cout << rayDir << "\n";
    //std::cout << "SC: " << sphereCentre << "\n";

    float root1 = -(b + pow(discriminant,0.5f))/(2.0f*a);
    float root2 = -(b - pow(discriminant,0.5f))/(2.0f*a);

    //std::cout << rayDir << "\n";
    //std::cout << root1 << " root1\n";
    //std::cout << root2 << " root2\n\n";

    //std::cout << root1 << " : " << root2 << "\n\n";
    if(root1 < 0 && root2 < 0)
        return std::numeric_limits<float>::quiet_NaN();
    if(!(root1 < 0) != !(root2 < 0))
        return 0.0f;
    return (root2<root1)?root2:root1;
}

float Collisions::rayPolyIntersect(sf::Vector2f rayStart,
                                  sf::Vector2f rayDir,
                                  std::vector<sf::Vertex > &poly,
                                  float tmin, float tmax, float epsilon)
{
    //sf::Vector2f r = rayStart;
    //sf::Vector2f v = rayDir;

    std::vector<Edge > potentialEdges;

    for(unsigned int i=0; i<poly.size(); ++i)
    {
        //std::cout << i << "\n";
        sf::Vector2f v0 = poly[i].position;
        sf::Vector2f v1 = poly[(i+1)%poly.size()].position;
        Edge polyEdge{v0, v1, v1-v0, v1};
        //sf::Vector2f edgeNorm = Collisions::orthogonal(v1-v0, 1.0f);

        //if(sfVectorMath::dot(rayDir, edgeNorm) < 0.0f)
        {
            //std::cout << polyEdge.dir << " v1:" << polyEdge.v1 << " i:" << i << "\n";
            potentialEdges.push_back(polyEdge);
        }


    }

    std::vector<float> edgeCollTimes;

    for(Edge &temp : potentialEdges)
    {
        float tColl = Collisions::rayEdgeIntersect(rayStart,
                                                   rayDir,
                                                   temp);
        edgeCollTimes.push_back(tColl);
    }

    bool movingAway = true;

    for(float &time : edgeCollTimes)
    {
        //std::cout << time << "\n";
        if(!std::isnan(time) && time < tmax)
            tmax = time;
        if(!std::isnan(time) && time > 0.0f)
            movingAway = false;
        //std::cout << tmax << "\n";
    }
    if(movingAway)
    {
        return std::numeric_limits<float>::quiet_NaN();
        std::cout << "mov away\n";
    }

    return tmax;

}

float Collisions::rayEdgeIntersect(sf::Vector2f rayStart,
                                  sf::Vector2f rayDir,
                                  Edge &edge1)
{
    sf::Vector2f r0 = rayStart;
    sf::Vector2f r1 = edge1.v1;
    sf::Vector2f d0 = rayDir;
    sf::Vector2f d1 = edge1.dir;

    float t1 = Math::cross(r0-r1, d0)/
               Math::cross(d1,d0);
    float t0 = Math::cross(r1-r0, d1)/
               Math::cross(d0,d1);

    if(t1 >= 0.0f && t1 <= 1.0f)
        return t0;
    return std::numeric_limits<float>::quiet_NaN();
}
