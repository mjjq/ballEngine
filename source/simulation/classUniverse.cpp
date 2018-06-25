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
        ballArray.back().setSamplePrevPosBool(enable_trajectories);
        //setPlayer(ballArray.size()-1);
        colliderArray.insertColumn(0);
        colliderArray.insertRow(0, std::numeric_limits<float>::quiet_NaN());
        //colliderArray.printMatrix();
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
            default:
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
        for(Ball &ball1 : ballArray)
            for(Ball &ball2 : ballArray)
                if(&ball1 != &ball2)
                    updateFirstVelocity(intEnum, dt, ball1, ball2);

    for(Ball &ball : ballArray)
        ball.updatePosition(dt);
}

/**
    Checks if the ball is about to intersect the world boundary and executes a
    damped collision.

    @param worldSizeX The x-component size of the simulation world.
    @param worldSizeY The y-component size of the simulation world.

    @return Void.
*/
bool BallUniverse::checkForBounce(Ball &ball)
{
    sf::Vector2f shapePos = ball.getPosition();
    float shapeRadius = ball.getRadius();
    sf::Vector2f velocity = ball.getVelocity();

    if(((shapePos.x+shapeRadius >= worldSizeX) && (velocity.x>=0))
    || ((shapePos.x-shapeRadius <= 0  && (velocity.x<=0))))
    {
        ball.setVelocity({-velocity.x, velocity.y});
        return true;
    }
    else if(((shapePos.y+shapeRadius >= worldSizeY) && (velocity.y>=0))
    || ((shapePos.y-shapeRadius <= 0  && (velocity.y<=0))))
    {
        ball.setVelocity({velocity.x, -velocity.y});
        return true;
    }
    return false;
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

    if(dot(relPos,relPos) <= pow(radSum, 2))
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

    for(unsigned int i=0;i<ballArray.size();++i)
    {
        for(unsigned int j=i;j<ballArray.size();++j)
        {
            if(i!=j)
            {
                //if(ballArray.at(i).getDistance(ballArray.at(j)) < 0.99*(ballArray.at(i).getRadius() + ballArray.at(j).getRadius()))
                //{
                //     std::cout << "Overlapping balls " << i << " " << j << " at " << ballArray.at(i).getPosition() << " \n";
                //     std::cout << colliderArray.getElementValue(i,j) << "\n";
                //}
                //if(ballArray.at(i).getHasCollided()==false && ballArray.at(j).getHasCollided()==false)
                //{
                //++num;
                //if(ballArray.at(i).getRadius() + 2.0*ballArray.at(i).getRelSpeed(ballArray.at(j))*dt //2.0 factor to prevent need for second call of this function
                 //+ ballArray.at(j).getRadius()
                 //> ballArray.at(i).getDistance(ballArray.at(j)))

                    float t_collPrime = timeToCollision(ballArray.at(i), ballArray.at(j));
                    colliderArray.setElementValue(i,j, t_collPrime);

                    if(t_collPrime < localT_coll)
                    {
                        localT_coll = t_collPrime;
                        coll1 = i;
                        coll2 = j;

                    }

                //else
                //    colliderArray.setElementValue(i,j, std::numeric_limits<float>::quiet_NaN());
                //}

            }
        }
        ballArray.at(i).setToCollided();
    }
    //std::cout << num << "\n";
    //std::cout << localT_coll << "\n";
    return std::make_tuple(coll1,coll2,localT_coll);
}

void BallUniverse::collTimeForBall(unsigned int index, float dt)
{
    for(unsigned int i=0; i<ballArray.size(); ++i)
    {
        if(index < i)
        {
            //if(ballArray.at(index).getRadius() +
            //   2.0*ballArray.at(index).getRelSpeed(ballArray.at(i))*dt >
            //   ballArray.at(index).getDistance(ballArray.at(i)))
            //{
                float tColl = timeToCollision(ballArray.at(index), ballArray.at(i));
                colliderArray.setElementValue(index, i, tColl);
            //}
            //else
            //    colliderArray.setElementValue(index, i, std::numeric_limits<float>::quiet_NaN());
        }
        else if(index > i)
        {
            float tColl = timeToCollision(ballArray.at(index), ballArray.at(i));
            colliderArray.setElementValue(i, index, tColl);
        }
    }
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

void BallUniverse::ballAbsorption(Ball &firstBall, Ball &secondBall, float dt)
{
    using namespace sfVectorMath;

    float rad1 = firstBall.getRadius();
    float rad2 = secondBall.getRadius();

    sf::Vector2f v1 = firstBall.getVelocity();
    sf::Vector2f v2 = secondBall.getVelocity();
    sf::Vector2f rhat = norm(firstBall.getPosition() - secondBall.getPosition());
    float projVel = dot(v2-v1, rhat)*dt;

    if(projVel > 0)
    {
        if(rad1 >= rad2)
        {
            rad2 -= projVel;
            rad1 = pow(rad1*rad1 + projVel*projVel , 0.5);
        }
        else if(rad2 >= rad1)
        {
            rad1 -= projVel;
            rad2 = pow(rad2*rad2 + projVel*projVel , 0.5);
        }
        firstBall.setRadius(rad1);
        firstBall.setOrigin({rad1,rad1});
        secondBall.setRadius(rad2);
        secondBall.setOrigin({rad2,rad2});
    }


    //ballCollision(firstBall, secondBall);
}

void BallUniverse::removeBall(unsigned int index)
{
    if(index >=0 && index < ballArray.size())
    {
        ballArray.erase(ballArray.begin() + index);
        colliderArray.removeColumn(index);
        colliderArray.removeRow(index);
        numOfBalls--;
    }
}

float BallUniverse::physicsLoop()
{
        float dtR = dt;
        float epsilon = 1e-5;

        if(playerInput.first)
            pushBall(0.1f, playerInput.second, currentPlayer);

        for(unsigned int i=0; i<ballArray.size(); ++i)
        {
            ballArray.at(i).resetToCollided();

            bool check = checkForBounce(ballArray.at(i));
            if(check)
                collTimeForBall(i, dt);
        }

        if(enable_collisions==true)
        {
            if(hasCollided==false)
                findShortestCollTime(timeToNextColl, ballArray, dt);

            else if(hasCollided==true)
                hasCollided = false;

            std::tuple<int,int,float> collTuple = colliderArray.getMatrixMin();
            timeToNextColl = std::get<2>(collTuple);
            collider1 = std::get<0>(collTuple);
            collider2 = std::get<1>(collTuple);
        }

        if(dt >= timeToNextColl && timeToNextColl >= epsilon)
        {
            dtR = timeToNextColl;
            if(std::abs(dtR) > epsilon)
            {
                updateAllObjects(enable_forces, std::floor(1e+3*dtR)/1e+3);

                if(collider1 != collider2)
                {
                    colliderArray.addConstValue(-dtR);
                    for(int i=0; i<ballArray.size(); ++i)
                        for(int j=i; j<ballArray.size(); ++j)
                            if(colliderArray.getElementValue(i,j) <= epsilon)
                            {
                                ballCollision(ballArray.at(i), ballArray.at(j));
                                collTimeForBall(i, dt);
                                collTimeForBall(j, dt);
                            }
                    hasCollided = true;
                }
            }
            timeToNextColl = 1e+15;
        }
        else
            updateAllObjects(enable_forces, dt);

        return dtR;
}

float BallUniverse::physicsLoopAbsorb()
{
        float dtR = dt;
        float epsilon = 1e-5;

        if(playerInput.first)
            pushBall(0.1f, playerInput.second, currentPlayer);

        for(unsigned int i=0; i<ballArray.size(); ++i)
        {
            ballArray.at(i).resetToCollided();
            //ball.checkForBounce(worldSizeX,worldSizeY);
            bool check = checkForBounce(ballArray.at(i));
            if(check)
            {
                std::cout << "true\n";
                collTimeForBall(i, dt);
            }
        }

        if(enable_collisions==true)
        {
            if(hasCollided==false)// && collAccumulator<=0.0f)
            {
                //std::tuple<int,int,float> collTuple =
                findShortestCollTime(timeToNextColl, ballArray, dt);
                //timeToNextColl = std::get<2>(collTuple);
                //collider1 = std::get<0>(collTuple);
                //collider2 = std::get<1>(collTuple);
                //collAccumulator = dt;
                //colliderArray.printMatrix();
                //std::cout << "false\n";
            }
            else if(hasCollided==true)
            {
                collTimeForBall(collider2, dt);
                collTimeForBall(collider1, dt);
                hasCollided = false;
                //colliderArray.printMatrix();
                //::cout << "true\n";
            }
            std::tuple<int,int,float> collTuple = colliderArray.getMatrixMin();
            timeToNextColl = std::get<2>(collTuple);
            collider1 = std::get<0>(collTuple);
            collider2 = std::get<1>(collTuple);
            //std::cout << timeToNextColl << "\n";
            //colliderArray.printMatrix();
            //std::cout << "\n";
        }

        if(dt >= std::floor(1e+3*timeToNextColl)/1e+3)
        {
            dtR = timeToNextColl;
            if(std::abs(dtR) > epsilon)
                updateAllObjects(enable_forces, std::floor(1e+3*dtR)/1e+3);

            if(collider1 != collider2)
            {
                //if(std::abs(dtR) < epsilon)
                //    sfVectorMath::printVector(ballArray.at(collider1).getVelocity());
                //ballArray.at(collider1).ballCollision(ballArray.at(collider2));
                ballCollision(ballArray.at(collider1), ballArray.at(collider2));
                //std::cout << "\ncollision" << collider1 << " " << collider2 << "\n";
                colliderArray.addConstValue(-dtR);
                /*ballAbsorption(ballArray.at(collider1), ballArray.at(collider2), dtR);
                if(ballArray.at(collider1).getRadius() < minBallSize)
                {
                    removeBall(collider1);
                    collTimeForBall(collider2, dt);
                }
                else if(ballArray.at(collider2).getRadius() < minBallSize)
                {
                    removeBall(collider2);
                    collTimeForBall(collider1, dt);
                }*/
                hasCollided = true;

                //colliderArray.printMatrix();
                //std::cout << "\n";
                //if(std::abs(dtR) < epsilon)
                //    sfVectorMath::printVector(ballArray.at(collider1).getVelocity());
            }
            timeToNextColl = 1e+15;
        }
        else
            updateAllObjects(enable_forces, dt);

        collAccumulator -= dtR;
        return dtR;
}

void BallUniverse::universeLoop(sf::Time frameTime, sf::Time frameLimit)
{
    accumulator += 120*dt*frameTime.asSeconds();//((frameTime<frameLimit)?frameTime:frameLimit).asSeconds();
    int limiting = 0;
    int maxLimit = 12;
    while(accumulator >= dt && limiting < maxLimit)
    {
        accumulator -= physicsLoop();
        sampleAllPositions();
        if(maxLimit*thresholdTimer.restart().asSeconds() > frameLimit.asSeconds())
            ++limiting;
    }
    playerInput.first = false;
    calcTotalKE(ballArray);
    calcTotalMomentum(ballArray);
    calcTotalGPE(ballArray);
    calcTotalEnergy();
    if( (limiting = maxLimit) && (accumulator >= dt) )
    {
        accumulator = 0.0f;
        std::cout << "Limit\n";
    }
}

void BallUniverse::sampleAllPositions()
{
    if(timeToNextSample - dt <= 0)
    {
        for(Ball &ball : ballArray)
            if(ball.getSamplePrevPosBool())
                ball.sampleNextPosition();

        timeToNextSample = sampledt;
        //std::cout << timeToNextSample << "\n";
    }
    else
        timeToNextSample -= dt;

    for(Ball &ball : ballArray)
        if(ball.getSamplePrevPosBool())
            ball.sampleCurrentPosition();
}

void BallUniverse::drawSampledPositions(sf::RenderWindow &window) //No longer very CPU intensive
{
    for(Ball &ball : ballArray)
    {
        if(ball.getSamplePrevPosBool())
        {
            std::deque<sf::Vector2f> previousPositions = ball.getPreviousPositions();
            if(previousPositions.size() > 1)
            {
                int colorGradient = previousPositions.size();
                sf::VertexArray lines(sf::LinesStrip, previousPositions.size());
                sf::Color color;
                for(unsigned int j=0; j<previousPositions.size(); ++j)
                {
                    if(ball.getIsPlayer())
                        color = {255,255,0,static_cast<sf::Uint8>(255*j/colorGradient)};
                    else
                        color = {255,255,255,static_cast<sf::Uint8>(255*j/colorGradient)};
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
    for(Ball &ball : ballArray)
        KE += ball.getKE();
    totalKE = KE;
}

void BallUniverse::calcTotalGPE(std::vector<Ball> &ballArray)
{
    totalGPE = 0;
    if(ballArray.size()>1)
    {
        float GPE{0};
        for(Ball &ball1 : ballArray)
            for(Ball &ball2 : ballArray)
                if(&ball1 != &ball2)
                    GPE+=ball1.getGPE(ball2)/2;
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
    for(Ball &ball : ballArray)
        momentum += ball.getMomentum();
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
    /*spawnNewBall(centralPosition, initVelocity, 50, 1000);
    spawnNewBall(centralPosition+sf::Vector2f{0.0f, 100.0f}, initVelocity+sf::Vector2f{3.0f, 0.0f}, 10, 1);
    spawnNewBall(centralPosition+sf::Vector2f{0.0f, -200.0f}, initVelocity+sf::Vector2f{-2.0f, 0.0f}, 10, 1);*/
    spawnNewBall({worldSizeX/2, worldSizeY/2}, {0,0}, 50, 1000);
    spawnNewBall({worldSizeX/2 + 200, worldSizeY/2}, {5,0}, 50, 1000);
    spawnNewBall({worldSizeX/2 - 205, worldSizeY/2}, {-5,0}, 50, 1000);
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
    //if(enable_trajectories)
    drawSampledPositions(windowRef);

    for(Ball &ball : ballArray)
        windowRef.draw(ball);
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
    for(Ball &ball : ballArray)
    {
        if(ball.getPosition().x > worldSizeX/2)
            ball.setFillColor(sf::Color::Red);
        else
            ball.setFillColor(sf::Color::Green);
    }
}

void BallUniverse::clearSimulation()
{
    ballArray.clear();
    colliderArray.clearMatrix();
    numOfBalls = 0;
    currentPlayer = -1;
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

sf::Vector2f BallUniverse::getBallPosition(unsigned int i)
{
    if(ballArray.size()>i && i>=0)
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
    if(ballArray.size()>0 && currentPlayer >= 0)
    {
        sf::Vector2f velocity = ballArray.at(i).getVelocity();
        sf::Vector2f forceVec{0,0};
        if(sfVectorMath::dot(velocity, velocity) != 0)
            forceVec = sfVectorMath::rotate(force*sfVectorMath::norm(velocity), relDirection);
        else
            forceVec = sfVectorMath::rotate({0,-force}, relDirection);
        ballArray.at(i).applyExternalImpulse(forceVec, dt);
    }
}

void BallUniverse::pushPlayer(float force, float relDirection)
{
    pushBall(force, relDirection, currentPlayer);
}

void BallUniverse::toggleTrajectories()
{
    if(enable_trajectories)
    {
        enable_trajectories = false;
        for(int i=0; (unsigned)i<ballArray.size(); ++i)
            if(i!=currentPlayer)
                ballArray.at(i).setSamplePrevPosBool(false);
    }
    else
    {
        enable_trajectories = true;
        for(int i=0; (unsigned)i<ballArray.size(); ++i)
            if(i!=currentPlayer)
                ballArray.at(i).setSamplePrevPosBool(true);
    }
}

void BallUniverse::setPlayer(unsigned int playerIndex)
{
    if(playerIndex < ballArray.size() && playerIndex >= 0)
    {
        if(currentPlayer >=0)
        {
            ballArray.at(currentPlayer).setIsPlayer(false);
            ballArray.at(currentPlayer).setFillColor(sf::Color::Green);
            //ballArray.at(currentPlayer).setSamplePrevPosBool(enable_trajectories);
        }
        ballArray.at(playerIndex).setIsPlayer(true);
        ballArray.at(playerIndex).setFillColor(sf::Color::Red);
        //ballArray.at(currentPlayer).setSamplePrevPosBool(enable_trajectories);
        currentPlayer = playerIndex;
    }
}

void BallUniverse::togglePlayerTraj()
{
    if(currentPlayer>=0)
    {
        Ball *currentBall = &ballArray.at(currentPlayer);
        if(currentBall->getSamplePrevPosBool())
            currentBall->setSamplePrevPosBool(false);
        else
            currentBall->setSamplePrevPosBool(true);
    }
}

void BallUniverse::splitBalls(int ballIndex, float relDirection, float speed)
{
    ballIndex = currentPlayer;
    float initialRadius = ballArray.at(ballIndex).getRadius();
    std::cout << initialRadius << "\n";
    float r2limit = 5;
    float r2factor = 0.15;
    float r2 = r2factor*initialRadius;
    if(r2 < r2limit)
        r2 = r2limit;
    if(r2 < initialRadius)
    {
        float r1 = pow(initialRadius*initialRadius - r2*r2, 0.5);
        if(r1 > r2)
        {
            sf::Vector2f initialVelocity = ballArray.at(ballIndex).getVelocity();
            sf::Vector2f v2;
            if(initialVelocity.x == 0 && initialVelocity.y == 0)
                v2 = speed*sfVectorMath::rotate({0,-1}, relDirection);
            else
                v2 = sfVectorMath::rotate(speed*sfVectorMath::norm(initialVelocity), relDirection);

            sf::Vector2f v1 = - (r2/r1)*(r2/r1)*v2;

            ballArray.at(ballIndex).setRadius(r1);
            ballArray.at(ballIndex).setOrigin({r1,r1});
            ballArray.at(ballIndex).setVelocity(v1 + initialVelocity);

            sf::Vector2f pos1 = sfVectorMath::rotate(1.01f*initialRadius*sfVectorMath::norm(initialVelocity), relDirection) +
                                ballArray.at(ballIndex).getPosition();
            spawnNewBall(pos1, v2 + initialVelocity, r2, r2);
        }
    }
}

void BallUniverse::playerInFunc(float relDirection)
{
    playerInput = std::pair<bool, float>(true, relDirection);
}

BallUniverse::BallUniverse(int worldSizeX, int worldSizeY, float dt, bool force, bool collision) :

worldSizeX{worldSizeX}, worldSizeY{worldSizeY}, enable_forces(force), enable_collisions(collision), dt{dt}
{

}
