#ifndef COLLDETECT_H
#define COLLDETECT_H

#include "classPhysicsObject.h"
#include "classBall.h"
#include "classAABB.h"
#include "classOBB.h"
#include "classPolygon.h"

//typedef std::pair<sf::Vector2f, sf::Vector2f> Edge;
struct Edge
{
    sf::Vector2f v1;
    sf::Vector2f v2;
    sf::Vector2f dir;
    sf::Vector2f max;
};

typedef std::vector<sf::Vector2f > ClippedPoints;

class Collisions
{
    static sf::RenderWindow* debugWindow;
public:
    static void setDebugWindow(sf::RenderWindow &window);

    static float rayAABBIntersect(sf::Vector2f rayStart, sf::Vector2f rayDir,
                      sf::Rect<float > &rectAABB, float tmin,
                      float tmax, float epsilon = 1e-5);
    static float raySphereIntersect(sf::Vector2f rayOrigin, sf::Vector2f rayDir,
                     sf::Vector2f sphereCentre, float sphereRadius);
    static float rayPolyIntersect(sf::Vector2f rayStart,
                                  sf::Vector2f rayDir,
                                  std::vector<sf::Vertex > &poly,
                                  float tmin, float tmax, float epsilon);
    static float rayEdgeIntersect(sf::Vector2f rayStart,
                                  sf::Vector2f rayDir,
                                  Edge &edge1);


    static float timeToCollision(PhysicsObject* p1, PhysicsObject* p2);
    static float timeToCollBallBall(Ball *firstBall, Ball *secondBall);
    static float timeToCollBallAABB(Ball* origBall, AABB* origAABB);
    static float timeToCollAABBAABB(AABB* rect1, AABB* rect2);
    static float timeToCollBallOBB(Ball *ball, OBB* rect);
    static float timeToCollOBBOBB(OBB* rect1, OBB* rect2);
    static float timeToCollOBBPoly(OBB* rect, Polygon* poly);
    static float timeToCollBallPoly(Ball* ball, Polygon* poly);
    static float timeToCollPolyPoly(Polygon* poly1, Polygon *poly2);

    static void resolveCollision(PhysicsObject* p1, PhysicsObject* p2);
    static void collisionBallBall(Ball* firstBall, Ball* secondBall);
    static void collisionBallAABB(Ball* origBall, AABB* origAABB);
    static void collisionAABBAABB(AABB* rect1, AABB* rect2);
    static void collisionBallOBB(Ball* ball, OBB* rect);
    static void collisionOBBOBB(OBB* rect1, OBB* rect2);
    static void collisionOBBPoly(OBB *rect, Polygon *poly);
    static void collisionBallPoly(Ball *ball, Polygon *poly);
    static void collisionPolyPoly(Polygon* poly1, Polygon *poly2);

    static sf::Vector2f calcPenetVector(sf::Vector2f rayStart, sf::Vector2f rayNorm, Ball &ball);
    static sf::Vector2f calcPenetVector(Ball* ball1, Ball* ball2);
    static sf::Vector2f calcPenetVector(AABB &rect1, AABB &rect2);

    static sf::Vector2f calcContactNorm(AABB &rect1, AABB &rect2);

    static sf::Rect<float > minkowskiDiff(AABB &rect1, AABB &rect2);
    static std::vector<sf::Vertex > minkowskiDiff(std::vector<sf::Vertex > &obj1,
                                                  std::vector<sf::Vertex > &obj2);

    static std::vector<sf::Vector2f > edgesOf(std::vector<sf::Vertex > &vertices);
    static std::pair<bool, sf::Vector2f > isSeparatingAxis(sf::Vector2f &orthog,
                                                            std::vector<sf::Vertex> &obj1,
                                                            std::vector<sf::Vertex> &obj2);
    static sf::Vector2f orthogonal(sf::Vector2f v, float scalar);
    static sf::Vector2f getCentre(std::vector<sf::Vertex> &obj);
    static std::pair<bool, sf::Vector2f> sepAxisTest(std::vector<sf::Vertex> &obj1,
                                          std::vector<sf::Vertex> &obj2);

    static void applyImpulse(PhysicsObject *obj1,
                             PhysicsObject *obj2,
                             sf::Vector2f contactNorm,
                             sf::Vector2f penetVector,
                             ClippedPoints &collisionPoints);


    static std::vector<sf::Vector2f > getContactPoints(std::vector<sf::Vertex > &obj1,
                                                       std::vector<sf::Vertex > &obj2,
                                                       sf::Vector2f contactNormal);
    static std::vector<sf::Vector2f > getContactPoints(std::vector<sf::Vertex > &obj1,
                                                        Ball &obj2,
                                                        sf::Vector2f contactNormal);
    static Edge getBestEdge(std::vector<sf::Vertex > &obj,
                     sf::Vector2f normal);

    static ClippedPoints clip(sf::Vector2f v1,
                               sf::Vector2f v2,
                               sf::Vector2f normal,
                               float o);

    static ClippedPoints getIntPoint(Ball &ball, Edge &edge);

    static std::pair<sf::Vector2f, sf::Vector2f> getContactNormal(Ball *origBall, AABB *origAABB);
    static std::pair<sf::Vector2f, sf::Vector2f> getContactNormal(Ball* ball, Polygon* poly);

    static int getClosestVertex(std::vector<sf::Vertex > &poly, sf::Vertex &intPoint);
};

#endif // COLLDETECT_H
