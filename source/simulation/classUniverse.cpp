#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "../../headers/classUniverse.h"
#include "../../headers/sfVectorMath.h"


void BallUniverse::spawnNewBall(sf::Vector2f position, sf::Vector2f velocity, float radius, float mass)
{
    if(!(position.x - radius < 0 || position.y - radius < 0
      || position.x + radius > worldSizeX || position.y + radius > worldSizeY))
    {
        ballArray.push_back(Ball(radius,mass,position,velocity));
        numOfBalls++;
    }
}

//void resetAndCheckBounce(std::vector<Ball>)

std::tuple<int,int,float> BallUniverse::findShortestCollTime(float t_coll, std::vector<Ball> &ballArray, float dt)
{
    int coll1 = 0; //collide function won't execute if collider1 = collider2 as this wouldn't make sense physically
    int coll2 = 0;
    //int num = 0;
    float localT_coll = t_coll;

    for(int i=0;i<ballArray.size();i++)
    {
        for(int j=0;j<ballArray.size();j++)
        {
            if(i!=j)
            {
                if(ballArray.at(i).getDistance(ballArray.at(j)) < 0.8*(ballArray.at(i).getRadius() + ballArray.at(j).getRadius()))
                    std::cout << "Overlapping balls at: " << ballArray.at(i).getPosition().x << "\n";
                if(ballArray.at(i).getHasCollided()==false && ballArray.at(j).getHasCollided()==false)
                {
                //++num;
                if(ballArray.at(i).getRadius() + 2.0*ballArray.at(i).getRelSpeed(ballArray.at(j))*dt //2.0 factor to prevent need for second call of this function
                 + ballArray.at(j).getRadius()
                 > ballArray.at(i).getDistance(ballArray.at(j)))
                {
                    //std::cout << "within distance: " << ballArray.at(i).getDistance(ballArray.at(j)) << "\n";
                    float t_collPrime = ballArray.at(i).timeToCollision(ballArray.at(j));
                    if(t_collPrime < localT_coll)
                    {
                        //std::cout << t_collPrime << " is smaller than " << t_coll << "\n";
                        localT_coll = t_collPrime;
                        coll1 = i;
                        coll2 = j;
                        //std::cout << localT_coll << "\n";
                        //ballArray.at(i).setToCollided();
                        //ballArray.at(j).setToCollided();

                    }
                }
                }

            }
        }
        ballArray.at(i).setToCollided();
    }
    //std::cout << num << "\n";
    //std::cout << localT_coll << "\n";
    return std::make_tuple(coll1,coll2,localT_coll);
}

void BallUniverse::physicsLoop()
{

        float dtR = dt;
        float epsilon = 1e-5;
        for(int i=0;i<ballArray.size();i++)
        {
            ballArray.at(i).resetToCollided();
            ballArray.at(i).checkForBounce(worldSizeX,worldSizeY);
        }

        if(enable_collisions==true)
        {
            std::tuple<int,int,float> collTuple = findShortestCollTime(timeToNextColl, ballArray, dt);
            timeToNextColl = std::get<2>(collTuple);
            collider1 = std::get<0>(collTuple);
            collider2 = std::get<1>(collTuple);
        }

        if(dt - timeToNextColl > 0)
        {
            dtR = timeToNextColl;
            if(std::abs(dtR) > epsilon)
                for(int i=0;i<ballArray.size();i++)
                {
                    ballArray.at(i).updatePosition(std::floor(1e+3*dtR)/1e+3);
                    //window.draw(ballArray.at(i));
                }
            if(collider1 != collider2)
            {
                //std::cout << collider1 << ":" << collider2 << " " << dtR << "\n";
                if(std::abs(dtR) < epsilon)
                    sfVectorMath::printVector(ballArray.at(collider1).getVelocity());
                ballArray.at(collider1).ballCollision(ballArray.at(collider2));
                if(std::abs(dtR) < epsilon)
                    sfVectorMath::printVector(ballArray.at(collider1).getVelocity());
                //ballArray.at(collider1).resetToCollided();
                //ballArray.at(collider2).resetToCollided();
            }
            timeToNextColl = 1e+15;
        }
        else
        {
            //std::cout <<"no collide";
            if(enable_forces==true)
            {
                for(int i=0;i<ballArray.size();i++)
                {
                    for(int j=0;j<ballArray.size();j++)
                    {
                        if(i!=j)
                            ballArray.at(i).updateVelocity(dt, ballArray.at(j));
                    }
                }
            }
            for(int i=0;i<ballArray.size();i++)
            {
                ballArray.at(i).updatePosition(dt);
                //window.draw(ballArray.at(i));
                //ballArray.at(i).resetToCollided();
            }
        }

}

void BallUniverse::universeLoop()
{
    if(!isPaused)
    {
        physicsLoop();
        calcTotalKE(ballArray);
        calcTotalMomentum(ballArray);
        //if(enable_trajectories)
        sampleAllPositions();
    }
    //drawSampledPositions();
}

void BallUniverse::sampleAllPositions()
{
    if(timeToNextSample - dt <= 0)
    {
        for(int i=0; i<ballArray.size(); i++)
            if(ballArray.at(i).getSamplePrevPosBool())
                ballArray.at(i).sampleNextPosition();

        timeToNextSample = sampledt;
        //std::cout << timeToNextSample << "\n";
    }
    else
        timeToNextSample -= dt;

    for(int i=0; i<ballArray.size(); i++)
        if(ballArray.at(i).getSamplePrevPosBool())
            ballArray.at(i).sampleCurrentPosition();
}

void BallUniverse::drawSampledPositions(sf::RenderWindow &window) //No longer very CPU intensive
{
    for(int i=0; i<ballArray.size(); i++)
    {
        if(ballArray.at(i).getSamplePrevPosBool())
        {
            std::deque<sf::Vector2f> previousPositions = ballArray.at(i).getPreviousPositions();
            if(previousPositions.size() > 1)
            {
                int colorGradient = previousPositions.size();
                sf::VertexArray lines(sf::LinesStrip, previousPositions.size());
                sf::Color color;
                for(int i=0; i<previousPositions.size(); i++)
                {
                    color = {255,255,255,255*i/colorGradient};
                    lines[i].position = previousPositions.at(i);
                    lines[i].color = color;
                    //line[0] = sf::Vertex(static_cast<sf::Vector2f>(previousPositions.at(i)),color);
                    //line[1] = sf::Vertex(static_cast<sf::Vector2f>(previousPositions.at(i+1)),color);
                    //window.draw(line, 2, sf::Lines);
                }
                window.draw(lines);
            }
        }
    }
}

void BallUniverse::calcTotalKE(std::vector<Ball> &ballArray)
{
    float KE{0};
    for(int i=0; i<ballArray.size(); i++)
        KE += ballArray.at(i).getKE();
    totalKE = KE;
}

void BallUniverse::calcTotalMomentum(std::vector<Ball> &ballArray)
{
    sf::Vector2f momentum{0,0};
    for(int i=0; i<ballArray.size(); i++)
        momentum+=ballArray.at(i).getMomentum();
    totalMomentum = momentum;
}

void BallUniverse::createBallGrid(int numWide, int numHigh, float spacing, sf::Vector2f centralPosition, sf::Vector2f init_velocity, float ballMass, float ballRadius)
{
    if(spacing < 2.5*ballRadius)
        spacing = 2.5*ballRadius;
    //if(!)
    {
        for(int i=-numWide/2; i<=numWide/2; i++)
            for(int j=-numHigh/2; j<=numHigh/2; j++)
            {
                sf::Vector2f offsetPosition = {i*spacing,j*spacing};
                spawnNewBall(centralPosition + offsetPosition, init_velocity, ballRadius, ballMass);
                //std::cout << i << " " << j << "\n";
            }
    }
}

sf::Vector2i BallUniverse::getWorldSize()
{
    sf::Vector2i wSize = {worldSizeX,worldSizeY};
    return wSize;
}

void BallUniverse::incSimStep(float delta)
{
    if(delta>0)
        dt+=delta;
}
void BallUniverse::decSimStep(float delta)
{
    if(delta>0 && dt>delta)
        dt-=delta;
}

void BallUniverse::setSimStep(float delta)
{
    if(delta > 0)
        dt = delta;
}

void BallUniverse::toggleSimPause()
{
    if(isPaused)
        isPaused = false;
        //setSimStep(previousDt);
    else
        isPaused = true;
        //setSimStep(0);
}

void BallUniverse::drawBalls(sf::RenderWindow &windowRef)
{
    if(enable_trajectories)
        drawSampledPositions(windowRef);
    for(int i=0; i<ballArray.size(); i++)
    {
        windowRef.draw(ballArray.at(i));
    }
}

void BallUniverse::toggleCollisions()
{
    if(enable_collisions)
        enable_collisions = false;
    else
        enable_collisions = true;
}

void BallUniverse::toggleForces()
{
    if(enable_forces)
        enable_forces = false;
    else
        enable_forces = true;
}

void BallUniverse::changeBallColour()
{
    for(int i=0; i<ballArray.size(); i++)
    {
        if(ballArray.at(i).getPosition().x > worldSizeX/2)
            ballArray.at(i).setFillColor(sf::Color::Red);
        else
            ballArray.at(i).setFillColor(sf::Color::Green);
    }
}

void BallUniverse::clearSimulation()
{
    ballArray.clear();
    numOfBalls = 0;
}

const int& BallUniverse::getWorldSizeX()
{
    return worldSizeX;
}

int& BallUniverse::getNumOfBalls()
{
    return numOfBalls;
}

bool& BallUniverse::getCollisionsEnabled()
{
    return enable_collisions;
}

bool& BallUniverse::getForcesEnabled()
{
    return enable_forces;
}

float& BallUniverse::getTotalKE()
{
    return totalKE;
}

sf::Vector2f& BallUniverse::getTotalMomentum()
{
    return totalMomentum;
}

sf::Vector2f BallUniverse::getBallPosition(int i)
{
    if(ballArray.size()>i)
        return ballArray.at(i).getPosition();
}

void BallUniverse::pushBall(float force, float relDirection, int i)
{
    sf::Vector2f velocity = ballArray.at(i).getVelocity();
    sf::Vector2f forceVec{0,0};
    if(sfVectorMath::dot(velocity, velocity) != 0)
        forceVec = sfVectorMath::rotate(force*sfVectorMath::norm(velocity), relDirection);
    else
        forceVec = sfVectorMath::rotate({0,-force}, relDirection);
    ballArray.at(i).applyExternalImpulse(forceVec, dt);
}

void BallUniverse::toggleTrajectories()
{
    if(enable_trajectories)
        enable_trajectories = false;
    else
        enable_trajectories = true;
}


BallUniverse::BallUniverse(int worldSizeX, int worldSizeY, float dt, bool force, bool collision) :

worldSizeX{worldSizeX}, worldSizeY{worldSizeY}, enable_forces(force), enable_collisions(collision), dt{dt}
{

}
