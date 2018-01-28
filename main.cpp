#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "classes.h"
#include "sfVectorMath.h"





int main()
{
    int windowSizeX = 700;
    int windowSizeY = 700;
    int worldSizeX = 700;
    int worldSizeY = 700;

    float dt=1;
    int spawnVelFactor = 10.f;
    float spawnRadius = 2;
    float spawnMass = 0.3;
    float ballGridSpacing = 8;
    int ballGridHeight = 10;
    int ballGridWidth = 10;

    bool enableForces = false;
    bool enableCollisions = true;

    BallUniverse universe1(windowSizeX, windowSizeY, worldSizeX, worldSizeY, dt, spawnVelFactor, spawnRadius, spawnMass,
                                    ballGridSpacing, ballGridHeight, ballGridWidth, enableForces, enableCollisions);

    universe1.universeMainLoop();

}
