#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "../../headers/classUniverse.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"


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
/**
    Updates the velocity of the current ball by calculating forces on the ball.

    @param dt The simulation timestep.
    @param &otherBall The other ball to interact with.

    @return Void.
*/
void BallUniverse::updateFirstVelocity(Integrators integType, float dt, Ball &firstBall, Ball &secondBall)
{
    sf::Vector2f relVec = firstBall.getPosition() - secondBall.getPosition();
    float r2 = sfVectorMath::dot(relVec, relVec);

    if(r2 > pow(firstBall.getRadius()+secondBall.getRadius(), 2))
    {
        //nStepVelocity = {0,0};
        float G = 1;
        float M = secondBall.getMass();
        sf::Vector2f firstVelocity = firstBall.getVelocity();
        sf::Vector2f secondVelocity = secondBall.getVelocity();
        std::pair<sf::Vector2f,sf::Vector2f> solution;
        switch(integType)
        {
            case(Integrators::INTEG_EULER):
                solution = integrators::eulerMethod(relVec, dt, M, G);
                break;
            case(Integrators::INTEG_SEM_IMP_EULER):
                solution = integrators::semImpEulerMethod(relVec, dt, M, G);
                break;
            case(Integrators::INTEG_RK4):
                solution = integrators::RK4Method2ndODE(relVec, firstVelocity, secondVelocity, dt, M, G);
                break;
            case(Integrators::INTEG_VERLET):
                solution = integrators::verletMethod(relVec, firstVelocity, secondVelocity, dt, M, G);
                break;
        }
        firstBall.addSolvedVelocity(solution.first, solution.second);
        /*cStepModVelocity += solution.first;
        nStepVelocity += solution.second;*/
    }

}

void BallUniverse::updateAllObjects(bool enableForces, float dt)
{
    if(enableForces==true)
        for(int i=0;i<ballArray.size();++i)
            for(int j=0;j<ballArray.size();++j)
                if(i!=j)
                    updateFirstVelocity(intEnum, dt, ballArray.at(i), ballArray.at(j));

    for(int i=0;i<ballArray.size();++i)
        ballArray.at(i).updatePosition(dt);
}


/**
    Calculate the time to collision with another ball.

    @param &otherBall Reference to the other ball.

    @return Time to collision.
*/
float BallUniverse::timeToCollision(Ball &firstBall, Ball &secondBall)
{
    using namespace sfVectorMath;

    sf::Vector2f relPos = firstBall.getPosition() - secondBall.getPosition();
    float radSum = firstBall.getRadius() + secondBall.getRadius();

    if(dot(relPos,relPos) < pow(radSum, 2))
        return std::numeric_limits<float>::quiet_NaN();

    sf::Vector2f relVel = firstBall.getVelocity() - secondBall.getVelocity();
    float discriminant = pow(dot(relPos,relVel), 2) -
                         dot(relVel,relVel)*(dot(relPos,relPos) - pow(radSum, 2));

    if(discriminant < 0)
        return std::numeric_limits<float>::quiet_NaN();

    float root1 = -(dot(relPos,relVel) + pow(discriminant,0.5))/dot(relVel,relVel);
    float root2 = -(dot(relPos,relVel) - pow(discriminant,0.5))/dot(relVel,relVel);

    if(root1 < 0 && root2 < 0)
        return std::numeric_limits<float>::quiet_NaN();
    else if(root1 < 0 && root2 > 0)
        return root2;
    else if(root2 < 0 && root1 > 0)
        return root1;
    return (root2<root1)?root2:root1;
}

std::tuple<int,int,float> BallUniverse::findShortestCollTime(float t_coll, std::vector<Ball> &ballArray, float dt)
{
    int coll1 = 0; //collide function won't execute if collider1 = collider2 as this wouldn't make sense physically
    int coll2 = 0;
    //int num = 0;
    float localT_coll = t_coll;

    for(int i=0;i<ballArray.size();++i)
    {
        for(int j=0;j<ballArray.size();++j)
        {
            if(i!=j)
            {
                /*if(ballArray.at(i).getDistance(ballArray.at(j)) < 0.8*(ballArray.at(i).getRadius() + ballArray.at(j).getRadius()))
                    std::cout << "Overlapping balls at: " << ballArray.at(i).getPosition().x << "\n";*/
                if(ballArray.at(i).getHasCollided()==false && ballArray.at(j).getHasCollided()==false)
                {
                //++num;
                if(ballArray.at(i).getRadius() + 2.0*ballArray.at(i).getRelSpeed(ballArray.at(j))*dt //2.0 factor to prevent need for second call of this function
                 + ballArray.at(j).getRadius()
                 > ballArray.at(i).getDistance(ballArray.at(j)))
                {
                    //std::cout << "within distance: " << ballArray.at(i).getDistance(ballArray.at(j)) << "\n";
                    //float t_collPrime = ballArray.at(i).timeToCollision(ballArray.at(j));
                    float t_collPrime = timeToCollision(ballArray.at(i), ballArray.at(j));
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

void BallUniverse::ballCollision(Ball &firstBall, Ball &secondBall)
{
    using namespace sfVectorMath;

    //sf::Vector2f relPos = firstBall.getPosition() - secondBall.getPosition();
    sf::Vector2f rhat = norm(firstBall.getPosition() - secondBall.getPosition());

    sf::Vector2f v1 = firstBall.getVelocity();
    sf::Vector2f v2 = secondBall.getVelocity();
    float m1 = firstBall.getMass();
    float m2 = secondBall.getMass();

    firstBall.setVelocity(v1 - rhat*dot(v1-v2,rhat)*(2*m2)/(m1+m2));
    secondBall.setVelocity(v2 + rhat*dot(v1-v2,rhat)*(2*m1)/(m1+m2));
}

float BallUniverse::physicsLoop()
{

        float dtR = dt;
        float epsilon = 1e-5;
        for(int i=0;i<ballArray.size();++i)
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
                updateAllObjects(enable_forces, std::floor(1e+3*dtR)/1e+3);

            if(collider1 != collider2)
            {
                if(std::abs(dtR) < epsilon)
                    sfVectorMath::printVector(ballArray.at(collider1).getVelocity());
                //ballArray.at(collider1).ballCollision(ballArray.at(collider2));
                ballCollision(ballArray.at(collider1), ballArray.at(collider2));
                if(std::abs(dtR) < epsilon)
                    sfVectorMath::printVector(ballArray.at(collider1).getVelocity());
            }
            timeToNextColl = 1e+15;
        }
        else
            updateAllObjects(enable_forces, dt);

        return dtR;
}

void BallUniverse::universeLoop(sf::Time frameTime, sf::Time frameLimit)
{
    if(!isPaused)
    {
        accumulator += 120*dt*((frameTime<frameLimit)?frameTime:frameLimit).asSeconds();
        int noOfInstances = 0;
        //sf::Clock clock;
        while(accumulator >= dt)
        {
            accumulator -= physicsLoop();
            ++noOfInstances;
        }
        calcTotalKE(ballArray);
        calcTotalMomentum(ballArray);
        calcTotalGPE(ballArray);
        calcTotalEnergy();
        //if(enable_trajectories)
        sampleAllPositions();

        /*if(clock.getElapsedTime()<frameLimit)
        {
            std::cout << (frameLimit-clock.getElapsedTime()).asSeconds() << "\n";
            sleep(frameLimit-clock.getElapsedTime());
        }*/

        //std::cout << noOfInstances << "\n";

    }
    //drawSampledPositions();
}

void BallUniverse::sampleAllPositions()
{
    if(timeToNextSample - dt <= 0)
    {
        for(int i=0; i<ballArray.size(); ++i)
            if(ballArray.at(i).getSamplePrevPosBool())
                ballArray.at(i).sampleNextPosition();

        timeToNextSample = sampledt;
        //std::cout << timeToNextSample << "\n";
    }
    else
        timeToNextSample -= dt;

    for(int i=0; i<ballArray.size(); ++i)
        if(ballArray.at(i).getSamplePrevPosBool())
            ballArray.at(i).sampleCurrentPosition();
}

void BallUniverse::drawSampledPositions(sf::RenderWindow &window) //No longer very CPU intensive
{
    for(int i=0; i<ballArray.size(); ++i)
    {
        if(ballArray.at(i).getSamplePrevPosBool())
        {
            std::deque<sf::Vector2f> previousPositions = ballArray.at(i).getPreviousPositions();
            if(previousPositions.size() > 1)
            {
                int colorGradient = previousPositions.size();
                sf::VertexArray lines(sf::LinesStrip, previousPositions.size());
                sf::Color color;
                for(int j=0; j<previousPositions.size(); ++j)
                {
                    color = {255,255,255,255*j/colorGradient};
                    lines[j].position = previousPositions.at(j);
                    lines[j].color = color;
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
    totalKE = 0;
    float KE{0};
    for(int i=0; i<ballArray.size(); ++i)
        KE += ballArray.at(i).getKE();
    totalKE = KE;
}

void BallUniverse::calcTotalGPE(std::vector<Ball> &ballArray)
{
    totalGPE = 0;
    if(ballArray.size()>1)
    {
        float GPE{0};
        for(int i=0; i<ballArray.size(); ++i)
            for(int j=0; j<ballArray.size(); ++j)
                if(i!=j)
                    GPE+=ballArray.at(i).getGPE(ballArray.at(j))/2;
        totalGPE = GPE;
    }
}

void BallUniverse::calcTotalEnergy()
{
    totalEnergy = totalKE + totalGPE;
}

void BallUniverse::calcTotalMomentum(std::vector<Ball> &ballArray)
{
    sf::Vector2f momentum{0,0};
    for(int i=0; i<ballArray.size(); ++i)
        momentum+=ballArray.at(i).getMomentum();
    totalMomentum = momentum;
}

void BallUniverse::createBallGrid(int numWide, int numHigh, float spacing, sf::Vector2f centralPosition, sf::Vector2f init_velocity, float ballMass, float ballRadius)
{
    if(spacing < 2.5*ballRadius)
        spacing = 2.5*ballRadius;
    //if(!)
    {
        for(int i=-numWide/2; i<=numWide/2; ++i)
            for(int j=-numHigh/2; j<=numHigh/2; ++j)
            {
                sf::Vector2f offsetPosition = {i*spacing,j*spacing};
                spawnNewBall(centralPosition + offsetPosition, init_velocity, ballRadius, ballMass);
                //std::cout << i << " " << j << "\n";
            }
    }
}

void BallUniverse::createAltBallGrid(int numWide, int numHigh, float spacing, sf::Vector2f centralPosition, sf::Vector2f init_velocity, float ballMass, float ballRadius)
{
    if(spacing < 2.5*ballRadius)
        spacing = 2.5*ballRadius;
    //if(!)
    {
        for(int i=-numWide/2; i<=numWide/2; ++i)
            for(int j=-numHigh/2; j<=numHigh/2; ++j)
            {
                sf::Vector2f offsetPosition = {i*spacing,j*spacing};
                spawnNewBall(centralPosition + offsetPosition, init_velocity, ballRadius, pow(-1,i)*ballMass);
                //std::cout << i << " " << j << "\n";
            }
    }
}

void BallUniverse::createSPSys(sf::Vector2f centralPosition, sf::Vector2f initVelocity)
{
    spawnNewBall(centralPosition, initVelocity, 50, 1000);
    spawnNewBall(centralPosition+sf::Vector2f{0.0f, 100.0f}, initVelocity+sf::Vector2f{3.0f, 0.0f}, 10, 1);
    spawnNewBall(centralPosition+sf::Vector2f{0.0f, -200.0f}, initVelocity+sf::Vector2f{-2.0f, 0.0f}, 10, 1);
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
    for(int i=0; i<ballArray.size(); ++i)
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

void BallUniverse::toggleRK4()
{
    int current = static_cast<int>(intEnum);
    int size = static_cast<int>(Integrators::LAST);
    current = (current+1)%size;
    //std::cout << "Int Type: " << current << "\n";
    intEnum = static_cast<Integrators>(current);
    //temp=temp+1;
}

void BallUniverse::changeBallColour()
{
    for(int i=0; i<ballArray.size(); ++i)
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

float& BallUniverse::getTotalEnergy()
{
    return totalEnergy;
}

sf::Vector2f& BallUniverse::getTotalMomentum()
{
    return totalMomentum;
}

sf::Vector2f BallUniverse::getBallPosition(int i)
{
    if(ballArray.size()>i && ballArray.size()>0)
        return ballArray.at(i).getPosition();
    return sf::Vector2f{std::numeric_limits<float>::quiet_NaN(),
                        std::numeric_limits<float>::quiet_NaN()};
}

float& BallUniverse::getTimeStep()
{
    return dt;
}

Integrators& BallUniverse::getUseRK4()
{
    return intEnum;
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
