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
    bool simFitsInWindow = true;

    float dt=1;
    int spawnVelFactor = 10;
    float spawnRadius = 2;
    float spawnMass = 0.5;
    float ballGridSpacing = 8;
    int ballGridHeight = 10;
    int ballGridWidth = 10;

    bool enableForces = true;
    bool enableCollisions = false;

    BallUniverse universe1(worldSizeX, worldSizeY, dt, enableForces, enableCollisions);
    NonSimulationStuff window1(windowSizeX, windowSizeY, spawnVelFactor, spawnRadius,
                                spawnMass, ballGridSpacing, ballGridHeight, ballGridWidth,
                                simFitsInWindow, universe1);

    window1.mainLoop();

}
