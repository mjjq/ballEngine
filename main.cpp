#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "./headers/classNonSim.h"
#include "./headers/sfVectorMath.h"
#include "./headers/stringConversion.h"

int main()
{
    int windowSizeX = 700;
    int windowSizeY = 700;
    int worldSizeX = 2000;
    int worldSizeY = 2000;
    bool simFitsInWindow = true;

    float dt=1;
    int spawnVelFactor = 10;
    float spawnRadius = 10;
    float spawnMass1 = 1.0;
    float ballGridSpacing = 8;
    int ballGridHeight = 10;
    int ballGridWidth = 10;

    bool enableForces = false;
    bool enableCollisions = true;

    BallUniverse universe1(worldSizeX, worldSizeY, dt, enableForces, enableCollisions);
    NonSimulationStuff window1(windowSizeX, windowSizeY, spawnVelFactor, spawnRadius,
                                spawnMass1, ballGridSpacing, ballGridHeight, ballGridWidth,
                                simFitsInWindow, universe1);

    window1.mainLoop();

}
