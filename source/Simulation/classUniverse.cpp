#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "classUniverse.h"
#include "sfVectorMath.h"
#include "stringConversion.h"

typedef std::map<ArbiterKey, Arbiter>::iterator ArbIter;
typedef std::pair<ArbiterKey, Arbiter> ArbPair;
typedef std::vector<std::unique_ptr<PhysicsObject> > PhysObjectArray;

void BallUniverse::spawnNewBall(ObjectProperties init)
{
    sf::Vector2f position = init._position;
    float radius = sqrtf(sfVectorMath::square(init._size));
    if(!(position.x - radius < 0 ||
         position.y - radius < 0 ||
         position.x + radius > worldSizeX ||
         position.y + radius > worldSizeY))
    {
        std::unique_ptr<Ball > newBall = std::make_unique<Ball >(init);

        dynamicObjects.push_back(std::move(newBall));
        numOfBalls++;
        //dynamicObjects.back()->setSamplePrevPosBool(enable_trajectories);
        //setPlayer(dynamicObjects.size()-1);
        //colliderArray.insertColumnQuick(std::numeric_limits<float>::quiet_NaN());
        //colliderArray.insertRow(0, std::numeric_limits<float>::quiet_NaN());
        //staticCollArray.insertRow(0, std::numeric_limits<float>::quiet_NaN());
        ////colliderArray.printMatrix();
        if(enable_collisions)
            calcCollTimes();
    }
}


/*void BallUniverse::spawnNewRect(sf::Vector2f position,
                                float width,
                                float height,
                                sf::Vector2f velocity,
                                float mass,
                                float rotation)
{
    if(!(position.x < 0 ||
       position.y < 0 ||
       position.x + width > worldSizeX ||
       position.y + height > worldSizeY))
    {
        std::unique_ptr<OBB > newRect = std::make_unique<OBB >(sf::Vector2f{width,height},
                                                                 mass,
                                                                 sf::Vector2f{position.x,
                                                                  position.y},
                                                                 velocity, rotation, 0.0f);
        dynamicObjects.push_back(std::move(newRect));

        numOfBalls++;
        //dynamicObjects.back()->setSamplePrevPosBool(enable_trajectories);
        //setPlayer(dynamicObjects.size()-1);
        //colliderArray.insertColumnQuick(std::numeric_limits<float>::quiet_NaN());
        //colliderArray.insertRow(0, std::numeric_limits<float>::quiet_NaN());
        //staticCollArray.insertRow(0, std::numeric_limits<float>::quiet_NaN());
        ////colliderArray.printMatrix();
        if(enable_collisions)
            calcCollTimes();
    }
}*/


void BallUniverse::spawnStaticBall(ObjectProperties init)
{
    sf::Vector2f position = init._position;
    float radius = sqrtf(sfVectorMath::square(init._size));
    if(!(position.x - radius < 0 ||
         position.y - radius < 0 ||
         position.x + radius > worldSizeX ||
         position.y + radius > worldSizeY))
    {
        init._mass = 1e+15;
        std::unique_ptr<Ball > newBall = std::make_unique<Ball >(init);
        staticObjects.push_back(std::move(newBall));

        //staticCollArray.insertColumnQuick(std::numeric_limits<float>::quiet_NaN());
        if(enable_collisions)
            calcCollTimes();
    }
}

/*void BallUniverse::spawnStaticRect(sf::Vector2f position, float width, float height, float rotation)
{
    if(!(position.x < 0 ||
       position.y < 0 ||
       position.x + width > worldSizeX ||
       position.y + height > worldSizeY))
    {
        std::unique_ptr<OBB > newRect = std::make_unique<OBB >(sf::Vector2f{width,height},
                                                                 1000000000000.0f,
                                                                 position+sf::Vector2f{width/2.0f, height/2.0f},
                                                                 sf::Vector2f{0.0f,0.0f},
                                                                 rotation, 0.0f);
        staticObjects.push_back(std::move(newRect));

        //staticCollArray.insertColumnQuick(std::numeric_limits<float>::quiet_NaN());
        if(enable_collisions)
            calcCollTimes();
    }
}*/


void BallUniverse::spawnNewPoly(ObjectProperties init)
{
    sf::Vector2f position = init._position;
    if(!(position.x < 0 ||
       position.y < 0 ||
       position.x> worldSizeX ||
       position.y> worldSizeY))
    {
        std::unique_ptr<Polygon > newPoly = std::make_unique<Polygon >(init);
        dynamicObjects.push_back(std::move(newPoly));

        numOfBalls++;
        //dynamicObjects.back()->setSamplePrevPosBool(enable_trajectories);
        //setPlayer(dynamicObjects.size()-1);
        //colliderArray.insertColumnQuick(std::numeric_limits<float>::quiet_NaN());
        //colliderArray.insertRow(0, std::numeric_limits<float>::quiet_NaN());
        //staticCollArray.insertRow(0, std::numeric_limits<float>::quiet_NaN());
        ////colliderArray.printMatrix();
        if(enable_collisions)
            calcCollTimes();
    }
}


void BallUniverse::spawnStaticPoly(ObjectProperties init)
{
    sf::Vector2f position = init._position;
    if(!(position.x < 0 ||
       position.y < 0 ||
       position.x> worldSizeX ||
       position.y> worldSizeY))
    {
        init._mass = 1e+15;
        std::unique_ptr<Polygon > newPoly = std::make_unique<Polygon >(init);

        staticObjects.push_back(std::move(newPoly));
        //staticCollArray.insertColumnQuick(std::numeric_limits<float>::quiet_NaN());
        if(enable_collisions)
            calcCollTimes();
    }
}

void BallUniverse::spawnNewObject(bool isStatic, SpawnObjectType type, ObjectProperties init)
{
    sf::Vector2f position = init._position;
    if(!(position.x < 0 ||
       position.y < 0 ||
       position.x> worldSizeX ||
       position.y> worldSizeY))
    {
        if(!isStatic)
        {
            switch(type)
            {
                case SpawnObjectType::Ball :
                {
                    std::unique_ptr<Ball > newBall = std::make_unique<Ball >(init);
                    dynamicObjects.push_back(std::move(newBall));
                    numOfBalls++;
                    break;
                }
                case SpawnObjectType::Polygon :
                {
                    std::unique_ptr<Polygon > newPoly = std::make_unique<Polygon >(init);
                    dynamicObjects.push_back(std::move(newPoly));
                    numOfBalls++;
                    break;
                }
                case SpawnObjectType::Capsule :
                {
                    std::unique_ptr<Capsule > newCapsule = std::make_unique<Capsule >(init);
                    dynamicObjects.push_back(std::move(newCapsule));
                    numOfBalls++;
                    break;
                }
                default:
                    break;
            }
        }
        else
        {
            init._mass = 1e+15;
            switch(type)
            {
                case SpawnObjectType::Ball:
                {
                    std::unique_ptr<Ball > newBall = std::make_unique<Ball >(init);
                    staticObjects.push_back(std::move(newBall));
                    break;
                }
                case SpawnObjectType::Polygon:
                {
                    std::unique_ptr<Polygon > newPoly = std::make_unique<Polygon >(init);
                    staticObjects.push_back(std::move(newPoly));
                    break;
                }
                case SpawnObjectType::Capsule:
                {
                    std::unique_ptr<Capsule > newCapsule = std::make_unique<Capsule >(init);
                    staticObjects.push_back(std::move(newCapsule));
                    break;
                }
                default:
                    break;
            }
        }
    }
}

void BallUniverse::spawnNewObject(std::unique_ptr<PhysicsObject> obj)
{
    dynamicObjects.emplace_back(std::move(obj));
    ++numOfBalls;
}


//void resetAndCheckBounce(std::vector<Ball>)
/**
    Updates the velocity of the current ball by calculating forces on the ball.

    @param dt The simulation timestep.
    @param &otherBall The other ball to interact with.

    @return Void.
*/
void BallUniverse::updateFirstVelocity(Integrators _integType, float _dt, PhysicsObject* obj1, PhysicsObject* obj2)
{
    sf::Vector2f relVec = obj1->getPosition() + obj1->getCoM() - (obj2->getPosition() + obj2->getCoM());
    float r2 = sfVectorMath::square(relVec);
    float totalR = obj1->getMinSize()+obj2->getMinSize();

    if(r2 > totalR*totalR)
    {
        //nStepVelocity = {0,0};
        float G = 1;
        float M = obj2->getMass();
        sf::Vector2f firstVelocity = obj1->getVelocity();
        sf::Vector2f secondVelocity = obj2->getVelocity();
        std::pair<sf::Vector2f,sf::Vector2f> solution;
        switch(_integType)
        {
            case(Integrators::INTEG_EULER):
                solution = integrators::eulerMethod(relVec, _dt, M, G);
                break;
            case(Integrators::INTEG_SEM_IMP_EULER):
                solution = integrators::semImpEulerMethod(relVec, _dt, M, G);
                break;
            case(Integrators::INTEG_RK4):
                solution = integrators::RK4Method2ndODE(relVec, firstVelocity, secondVelocity, _dt, M, G);
                break;
            case(Integrators::INTEG_VERLET):
                solution = integrators::verletMethod(relVec, firstVelocity, secondVelocity, _dt, M, G);
                break;
            case(Integrators::LAST):
            default:
                break;
        }
        obj1->addSolvedVelocity(solution.first, solution.second);
        /*cStepModVelocity += solution.first;
        nStepVelocity += solution.second;*/
    }

}

void BallUniverse::updateAllObjects(bool _enableForces, float _dt)
{
    if(_enableForces==true)
        for(auto iter1 = dynamicObjects.begin(); iter1 != dynamicObjects.end(); ++iter1)
            for(auto iter2 = dynamicObjects.begin(); iter2 != dynamicObjects.end(); ++iter2)
                if(&iter1 != &iter2 && !(*iter1)->ignoresGravity() && !(*iter2)->ignoresGravity())
                    updateFirstVelocity(intEnum, _dt, (*iter1).get(), (*iter2).get());

    if(universalGravity==true)
        for(auto iter = dynamicObjects.begin(); iter != dynamicObjects.end(); iter++)
        {
            if(!(*iter)->ignoresGravity())
            {
                std::pair<sf::Vector2f,sf::Vector2f> solution;
                solution = integrators::verletMethod(_dt, uGravityDir);
                (*iter)->addSolvedVelocity(solution.first, solution.second);
            }
        }
}

/**
    Checks if the ball is about to intersect the world boundary and executes a
    damped collision.

    @param worldSizeX The x-component size of the simulation world.
    @param worldSizeY The y-component size of the simulation world.

    @return Void.
*/
bool BallUniverse::checkForBounce(PhysicsObject* object)
{
    sf::Vector2f shapePos = object->getPosition();
    float shapeRadius = 0.0f;
    sf::Vector2f velocity = object->getVelocity();

    if(((shapePos.x+shapeRadius >= worldSizeX) && (velocity.x>=0))
    || ((shapePos.x-shapeRadius <= 0  && (velocity.x<=0))))
    {
        object->setVelocity({-velocity.x, velocity.y});
        return true;
    }
    else if(((shapePos.y+shapeRadius >= worldSizeY) && (velocity.y>=0))
    || ((shapePos.y-shapeRadius <= 0  && (velocity.y<=0))))
    {
        object->setVelocity({velocity.x, -velocity.y});
        return true;
    }
    return false;
}



void BallUniverse::calcCollTimes()
{
    for(unsigned int i=0;i<colliderArray.getHeight();++i)
        for(unsigned int j=i+1;j<colliderArray.getWidth();++j)
            //if(i!=j)
            {
                /*if(dynamicObjects.at(i).getDistance(dynamicObjects.at(j)) < 0.99*(dynamicObjects.at(i).getRadius() + dynamicObjects.at(j).getRadius()))
                {
                     std::cout << "Overlapping balls " << i << " " << j << " at " << dynamicObjects.at(i).sf::CircleShape::getPosition() << " \n";
                     std::cout << //colliderArray.getElementValue(i,j) << "\n";
                }*/
                    float tColl = Collisions::timeToCollision(dynamicObjects.at(i).get(), dynamicObjects.at(j).get());
                    //colliderArray.setElementValue(j,i, tColl);

            }
    for(unsigned int i=0; i<staticCollArray.getHeight(); ++i)
        for(unsigned int j=0; j<staticCollArray.getWidth(); ++j)
        {
            float tColl = Collisions::timeToCollision(dynamicObjects.at(i).get(), staticObjects.at(j).get());
            //staticCollArray.setElementValue(j,i, tColl);
        }
}


void BallUniverse::findShortestCollTime()
{
    int xDyn = 0;
    int yDyn = 0;
    int xStat = 0;
    int yStat = 0;
    float collTupleDyn = colliderArray.getMatrixMin(xDyn, yDyn);
    float collTupleStat = staticCollArray.getMatrixMin(xStat, yStat);
    ////colliderArray.printMatrix(); std::cout << "\n";
    ////staticCollArray.printMatrix(); std::cout << "\n";
    if(collTupleDyn < collTupleStat)
    {
        //std::cout << "Dyn\n";
        collWithStatic = false;
        timeToNextColl = collTupleDyn;
        collider1 = xDyn;
        collider2 = yDyn;
    }
    else
    {
        //std::cout << "stat\n";
        collWithStatic = true;
        timeToNextColl = collTupleStat;
        collider1 = xStat;
        collider2 = yStat;
    }
    //std::cout << timeToNextColl << ": " << collider1 << " " << collider2 << "\n";
}

void BallUniverse::collTimeForBall(unsigned int index)
{
    for(unsigned int i=0; i<dynamicObjects.size(); ++i)
    {
        if(index < i)
        {
            //if(dynamicObjects.at(index).getRadius() +
            //   2.0*dynamicObjects.at(index).getRelSpeed(dynamicObjects.at(i))*dt >
            //   dynamicObjects.at(index).getDistance(dynamicObjects.at(i)))
            //{
                float tColl = Collisions::timeToCollision(dynamicObjects.at(index).get(), dynamicObjects.at(i).get());
                //colliderArray.setElementValue(i, index, tColl);
            //}
            //else
            //    //colliderArray.setElementValue(index, i, std::numeric_limits<float>::quiet_NaN());
        }
        else if(index > i)
        {
            float tColl = Collisions::timeToCollision(dynamicObjects.at(index).get(), dynamicObjects.at(i).get());
            //colliderArray.setElementValue(index, i, tColl);
        }
    }
    for(unsigned int i=0; i<staticCollArray.getWidth(); ++i)
    {
        float tColl = Collisions::timeToCollision(dynamicObjects.at(index).get(), staticObjects.at(i).get());
        //staticCollArray.setElementValue(i, index, tColl);
    }
}



/*void BallUniverse::ballAbsorption(Ball &_firstBall, Ball &_secondBall)
{
    using namespace sfVectorMath;

    float rad1 = _firstBall.getRadius();
    float rad2 = _secondBall.getRadius();

    float projVel = dot( _secondBall.getVelocity() - _firstBall.getVelocity(),
                         norm(_firstBall.getPosition() - _secondBall.getPosition()) );

    if(projVel > 0)
    {
        if(projVel < rad1 && projVel < rad2)
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
        }
        else
        {
            if(rad1 >= rad2)
            {
                rad2 = 0;
                rad1 = pow(rad1*rad1 + rad2*rad2 , 0.5);
            }
            else if(rad2 >= rad1)
            {
                rad1 = 0;
                rad2 = pow(rad2*rad2 + rad1*rad1 , 0.5);
            }
        }
        _firstBall.setRadius(rad1);
        _firstBall.setOrigin({rad1,rad1});
        _firstBall.setMass(3.14159265359*rad1*rad1*_firstBall.getDensity());
        _secondBall.setRadius(rad2);
        _secondBall.setOrigin({rad2,rad2});
        _secondBall.setMass(3.14159265359*rad2*rad2*_secondBall.getDensity());
    }


    Collisions::ballCollision(_firstBall, _secondBall);
}*/

void BallUniverse::removeBall(int index)
{
    if(std::abs(index) < (int)dynamicObjects.size())
    {
        if(index >=0)
        {
            dynamicObjects.erase(dynamicObjects.begin() + index);

        }
        else if(index < 0)
        {

            dynamicObjects.erase(dynamicObjects.end() + index + 1);
        }

        numOfBalls--;
        //colliderArray.removeEndRow();
        //colliderArray.removeColumnQuick(std::numeric_limits<float>::quiet_NaN());

        ////staticCollArray.removeEndRow();

        if(enable_collisions)
            calcCollTimes();
    }
    else if(index == -1 && dynamicObjects.size() == 1)
    {
        numOfBalls = 0;
        dynamicObjects.clear();
        //colliderArray.clearMatrix();

        ////staticCollArray.removeEndRow();
    }

    arbiters.clear();
    //broadPhase();
}

void BallUniverse::removeRect(int index)
{
    if(std::abs(index) < (int)staticObjects.size())
    {
        if(index >= 0)
            staticObjects.erase(staticObjects.begin() + index);
        else if(index < 0)
            staticObjects.erase(staticObjects.end() + index + 1);

        //staticCollArray.removeColumnQuick(std::numeric_limits<float>::quiet_NaN());

        if(enable_collisions)
            calcCollTimes();
    }
    else if(index == -1 && staticObjects.size() == 1)
    {
        staticObjects.clear();
        //staticCollArray.removeColumnQuick(std::numeric_limits<float>::quiet_NaN());
    }

    arbiters.clear();
    broadPhase();
}

void BallUniverse::broadPhase()
{
    float dtR = dt;
    for(unsigned int i=0; i<dynamicObjects.size(); ++i)
    {
        PhysicsObject* obji = dynamicObjects[i].get();

        for(unsigned int j=i+1; j<dynamicObjects.size(); ++j)
        {
            //std::cout << i << " " << j << "\n";
            PhysicsObject* objj = dynamicObjects[j].get();

            Arbiter newArb(obji, objj);
            ArbiterKey key(obji, objj);


            if(newArb.numContacts > 0)
            {
                ArbIter iter = arbiters.find(key);

                if(iter == arbiters.end())
                {
                    arbiters.insert(ArbPair(key, newArb));
                }
                else
                    iter->second.update();
            }
            else
            {
                if(obji->isBullet() || objj->isBullet())
                {
                    //float newDt = Collisions::timeToCollision(obji, objj);
                    //if(newDt < dtR)
                }
                else
                    arbiters.erase(key);
            }
        }
    }

    for(unsigned int i=0; i<dynamicObjects.size(); ++i)
    {
        PhysicsObject* obji = dynamicObjects[i].get();

        for(unsigned int j=0; j<staticObjects.size(); ++j)
        {
            PhysicsObject* objj = staticObjects[j].get();

            Arbiter newArb(obji, objj);
            ArbiterKey key(obji, objj);

            if(newArb.numContacts > 0)
            {
                ArbIter iter = arbiters.find(key);

                if(iter == arbiters.end())
                {
                    arbiters.insert(ArbPair(key, newArb));
                }
                else
                    iter->second.update();
            }
            else
                arbiters.erase(key);
        }
    }
}

float BallUniverse::physicsLoop()
{
    //float dtR = dt;
    //float epsilon = 1e-5;


    for(unsigned int i=0; i<dynamicObjects.size(); ++i)
    {
        //dynamicObjects[i].resetToCollided();
        checkForBounce(dynamicObjects[i].get());
        //if( checkForBounce(dynamicObjects[i].get()) && enable_collisions)
            //collTimeForBall(i);
    }

    broadPhase();

    universeSub.notify(*this, Event{EventType::Character_Contact});
    universeSub.notify(*this, Event{EventType::Projectile_Contact});

    updateAllObjects(enable_forces, dt);

    for(ArbIter arb = arbiters.begin(); arb != arbiters.end(); ++arb)
    {
        arb->second.PreStep(1.0f/dt);
    }
    for(auto jointIt = joints.begin(); jointIt != joints.end(); ++jointIt)
    {
        jointIt->PreStep(1.0f/dt);
    }

    for (int i = 0; i < 10; ++i)
    {
        for (ArbIter arb = arbiters.begin(); arb != arbiters.end(); ++arb)
        {
            arb->second.ApplyImpulse();
        }
        for(auto jointIt = joints.begin(); jointIt != joints.end(); ++jointIt)
        {
            jointIt->ApplyImpulse();
        }
    }
    //std::cout << "hello\n";
    //std::cout << arbiters.size() <<"\n";

    for(unsigned int i=0; i<dynamicObjects.size(); ++i)
    {
        dynamicObjects[i].get()->updatePosition(dt);
    }

    return dt;
}

/*float BallUniverse::physicsLoopAbsorb()
{
        float dtR = dt;
        float epsilon = 1e-5;

        pushBall(playerInput, currentPlayer);

        for(unsigned int i=0; i<dynamicObjects.size(); ++i)
        {
            dynamicObjects[i].resetToCollided();
            if( checkForBounce(dynamicObjects[i]) )
                collTimeForBall(i);
        }

        if(enable_collisions==true)
        {
            if(hasCollided==false)
                calcCollTimes();

            else if(hasCollided==true)
            {
                collTimeForBall(collider2);
                if(!collWithStatic)
                    collTimeForBall(collider1);
                hasCollided = false;
            }

            findShortestCollTime();
        }

        if(dt >= std::floor(1e+3*timeToNextColl)/1e+3)
        {
            hasCollided = true;
            dtR = timeToNextColl;
            if(std::abs(dtR) > epsilon)
                updateAllObjects(enable_forces, std::floor(1e+3*dtR)/1e+3);

            //colliderArray.addConstValue(-dtR);
            //staticCollArray.addConstValue(-dtR);

            if(collWithStatic)
                Collisions::ballCollision(dynamicObjects[collider2], staticObjects[collider1]);

            else if(collider1 != collider2)
            {
                ballAbsorption(dynamicObjects[collider1], dynamicObjects[collider2]);
                if(dynamicObjects[collider1].getRadius() < 0.01f)
                {
                    hasCollided = false;
                    removeBall(collider1);
                    if(collider2 > collider1)
                        collider2--;
                }
                if(dynamicObjects[collider2].getRadius() < 0.01f)
                {
                    hasCollided = false;
                    removeBall(collider2);
                    if(collider2 < collider1)
                        collider1--;
                }
            }

            timeToNextColl = 1e+15;
        }
        else
            updateAllObjects(enable_forces, dt);

        collAccumulator -= dtR;
        return dtR;
}*/

void BallUniverse::universeLoop(sf::Time frameTime, sf::Time frameLimit)
{
    if(!isPaused)
    {
        accumulator += 120*dt*frameTime.asSeconds();//((frameTime<frameLimit)?frameTime:frameLimit).asSeconds();
        int limiting = 0;
        int maxLimit = 1000;
        float dtR = dt;

        while(accumulator >= dt && limiting < maxLimit)
        {
            thresholdTimer.restart();
            //std::cout << "Before: " << limiting << "\n";
            dtR = physicsLoop();
            accumulator -= dtR;
            //std::cout << "After: " << limiting << "\n";
            //std::cout << "After: " << thresholdTimer.getElapsedTime().asSeconds() << "\n\n";

            sampleAllPositions();
            if(thresholdTimer.getElapsedTime().asSeconds() > frameLimit.asSeconds()*dtR)
                ++limiting;
        }
        //playerInput.first = false;
        //calcTotalKE(dynamicObjects);
        //calcTotalMomentum(dynamicObjects);
        //calcTotalGPE(dynamicObjects);
        //calcTotalEnergy();
        if( (limiting == maxLimit) && (accumulator >= dt) )
        {
            accumulator = 0.0f;
            std::cout << "Limit\n";
            if(frameTime.asSeconds() > 1.0f)
                isPaused = true;
        }
    }
}

void BallUniverse::sampleAllPositions()
{
    if(timeToNextSample - dt <= 0)
    {
        for(auto iter = dynamicObjects.begin(); iter != dynamicObjects.end(); ++iter)
            if((**iter).getSamplePrevPosBool())
                (**iter).sampleNextPosition();

        timeToNextSample = sampledt;
        //std::cout << timeToNextSample << "\n";
    }
    else
        timeToNextSample -= dt;

    for(auto iter = dynamicObjects.begin(); iter != dynamicObjects.end(); ++iter)
        if((**iter).getSamplePrevPosBool())
            (**iter).sampleCurrentPosition();
}

void BallUniverse::drawSampledPositions(sf::RenderWindow &window) //No longer very CPU intensive
{
    for(auto iter = dynamicObjects.begin(); iter != dynamicObjects.end(); ++iter)
    {
        if((**iter).getSamplePrevPosBool())
        {
            std::deque<sf::Vector2f> previousPositions = (**iter).getPreviousPositions();
            if(previousPositions.size() > 1)
            {
                int colorGradient = previousPositions.size();
                sf::VertexArray lines(sf::LinesStrip, previousPositions.size());
                sf::Color color;
                for(unsigned int j=0; j<previousPositions.size(); ++j)
                {
                    if((**iter).getIsPlayer())
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

void BallUniverse::calcTotalKE(std::vector<PhysicsObject* > &_dynamicObjects)
{
    totalKE = 0;
    float KE{0};
    for(auto iter1 = _dynamicObjects.begin(); iter1 != _dynamicObjects.end(); ++iter1)
        KE += (**iter1).getKE();
    totalKE = KE;
}

void BallUniverse::calcTotalGPE(std::vector<PhysicsObject* > &_dynamicObjects)
{
    totalGPE = 0;
    if(_dynamicObjects.size()>1)
    {
        float GPE{0};
        for(auto iter1 = _dynamicObjects.begin(); iter1 != _dynamicObjects.end(); ++iter1)
            for(auto iter2 = _dynamicObjects.begin(); iter2 != _dynamicObjects.end(); ++iter2)
                if(&iter1 != &iter2)
                    GPE+=(**iter1).getGPE(*iter2)/2.0f;
        totalGPE = GPE;
    }
}

void BallUniverse::calcTotalEnergy()
{
    totalEnergy = totalKE + totalGPE;
}

void BallUniverse::calcTotalMomentum(std::vector<PhysicsObject* > &_dynamicObjects)
{
    sf::Vector2f momentum{0,0};
    for(auto iter1 = _dynamicObjects.begin(); iter1 != _dynamicObjects.end(); ++iter1)
        momentum += (**iter1).getMomentum();
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
                spawnNewBall({centralPosition + offsetPosition,
                             init_velocity,
                             {ballRadius, 0.0f},
                             ballMass,
                             0.0f,
                             1.0f,
                             0.0f,
                             0.0f});
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
                //sf::Vector2f offsetPosition = {i*spacing,j*spacing};
                //spawnNewBall(centralPosition + offsetPosition, init_velocity, ballRadius, pow(-1,i)*ballMass);
                //std::cout << i << " " << j << "\n";
            }
    }
}

void BallUniverse::createSPSys(sf::Vector2f centralPosition, sf::Vector2f initVelocity)
{
    //spawnNewBall(centralPosition, initVelocity, 50, 1000);
    //spawnNewBall(centralPosition+sf::Vector2f{0.0f, 100.0f}, initVelocity+sf::Vector2f{3.0f, 0.0f}, 10, 1);
    //spawnNewBall(centralPosition+sf::Vector2f{0.0f, -200.0f}, initVelocity+sf::Vector2f{-2.0f, 0.0f}, 10, 1);
    /*spawnNewBall({worldSizeX/2.0f, worldSizeY/2.0f}, {0,0}, 50, 1000);
    spawnNewBall({worldSizeX/2.0f + 200.0f, worldSizeY/2}, {5,0}, 50, 1000);
    spawnNewBall({worldSizeX/2 - 201, worldSizeY/2}, {-5,0}, 50, 1000);*/
    //spawnStaticRect(centralPosition, 300.0f*initVelocity.x, 300.0f*initVelocity.y);
    //spawnNewRect(centralPosition, 50.0f, 50.0f, initVelocity, 5.0f);
    //std::cout << "Size: " << staticObjects.size() << "\n";
    ////staticCollArray.printMatrix();
    //std::cout << "\n";
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

    for(auto iter = dynamicObjects.begin(); iter != dynamicObjects.end(); ++iter)
        (**iter).draw(windowRef);

    for(auto iter = staticObjects.begin(); iter != staticObjects.end(); ++iter)
        (**iter).draw(windowRef);
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

void BallUniverse::toggleUGravity()
{
    if(universalGravity)
        universalGravity = false;
    else
        universalGravity = true;
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
    /*for(Ball &ball : dynamicObjects)
    {
        if(ball.sf::CircleShape::getPosition().x > worldSizeX/2)
            ball.setFillColor(sf::Color::Red);
        else
            ball.setFillColor(sf::Color::Green);
    }*/
}

void BallUniverse::clearSimulation()
{
    dynamicObjects.clear();
    staticObjects.clear();
    //colliderArray.clearMatrix();
    //staticCollArray.clearMatrix();
    numOfBalls = 0;
    arbiters.clear();
    joints.clear();
}

const int& BallUniverse::getWorldSizeX()
{
    return worldSizeX;
}

std::string BallUniverse::getNumOfBalls()
{

    return std::to_string(numOfBalls);
}

std::string BallUniverse::getCollisionsEnabled()
{
    return std::to_string(enable_collisions);
}

std::string BallUniverse::getUGravityEnabled()
{
    return std::to_string(universalGravity);
}

std::string BallUniverse::getForcesEnabled()
{
    return std::to_string(enable_forces);
}

std::string BallUniverse::getTotalKE()
{
    return std::to_string(totalKE);
}

std::string BallUniverse::getTotalEnergy()
{
    return std::to_string(totalEnergy);
}

std::string BallUniverse::getTotalMomentum()
{
    return to_string(totalMomentum);
}

sf::Vector2f BallUniverse::getObjPosition(unsigned int i)
{
    if(dynamicObjects.size()>i && i>=0)
        return dynamicObjects.at(i).get()->getPosition();
    return sf::Vector2f{std::numeric_limits<float>::quiet_NaN(),
                        std::numeric_limits<float>::quiet_NaN()};
}

std::string BallUniverse::getTimeStep()
{
    return std::to_string(dt);
}

std::string BallUniverse::getUseRK4()
{
    return to_string(intEnum);
}

std::string BallUniverse::getBallSpeed(unsigned int index)
{
    if(index < dynamicObjects.size() && index >= 0)
        return std::to_string(dynamicObjects.at(index).get()->getSpeed());

    return "dynamicObjects index out of range";
}

int BallUniverse::getNumTimesColld(unsigned int index)
{
    if(index < dynamicObjects.size() && index >= 0)
        return dynamicObjects.at(index).get()->getNumCollTimes();

    return -1;
}


void BallUniverse::toggleTrajectories()
{
    if(enable_trajectories)
    {
        enable_trajectories = false;
        for(int i=0; (unsigned)i<dynamicObjects.size(); ++i)
            dynamicObjects.at(i).get()->setSamplePrevPosBool(false);
    }
    else
    {
        enable_trajectories = true;
        for(int i=0; (unsigned)i<dynamicObjects.size(); ++i)
            dynamicObjects.at(i).get()->setSamplePrevPosBool(true);
    }
}

void BallUniverse::togglePlayerTraj()
{
}

void BallUniverse::splitBalls(int ballIndex, float relDirection, float speed)
{
    /*ballIndex = currentPlayer;
    float initialRadius = dynamicObjects.at(ballIndex).getRadius();
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
            sf::Vector2f initialVelocity = dynamicObjects.at(ballIndex).getVelocity();
            sf::Vector2f v2;
            if(initialVelocity.x < 1e-10 && initialVelocity.y < 1e-10)
                v2 = speed*sfVectorMath::rotate({0,-1}, relDirection);
            else
                v2 = sfVectorMath::rotate(speed*sfVectorMath::norm(initialVelocity), relDirection);

            sf::Vector2f v1 = - (r2/r1)*(r2/r1)*v2;

            dynamicObjects.at(ballIndex).setRadius(r1);
            dynamicObjects.at(ballIndex).setOrigin({r1,r1});
            dynamicObjects.at(ballIndex).setVelocity(v1 + initialVelocity);

            sf::Vector2f pos1 = sfVectorMath::rotate(1.01f*initialRadius*sfVectorMath::norm(initialVelocity), relDirection) +
                                dynamicObjects.at(ballIndex).sf::CircleShape::getPosition();
            spawnNewBall(pos1, v2 + initialVelocity, r2, r2);
        }
    }*/
}

void BallUniverse::applyUGravity()
{
    for(auto iter = dynamicObjects.begin(); iter != dynamicObjects.end(); ++iter)
    {
        //(*iter)->applyExternalImpulse(uGravityDir, dt);
    }
}


void BallUniverse::createExplosion(sf::Vector2f position,
                         float radiusOfEffect,
                         float strength)
{
    ObjectProperties tempProps;
    tempProps._position = position;
    tempProps._vertices = {sf::Vertex{{0.0f, 0.0f}}};

    for(int i=0; i<dynamicObjects.size(); ++i)
    {
        Polygon tempObject(tempProps);
        tempObject.setMomentInertia(1.0f);

        Edge GJKResult = GJK::getClosestPoints(dynamicObjects[i].get(), &tempObject);

        float distanceSq = sfVectorMath::square(GJKResult.v2 - GJKResult.v1);
        if(distanceSq < radiusOfEffect*radiusOfEffect && distanceSq > 0.0f)
        {
            float factor = std::min(strength, strength/sqrtf(distanceSq));
            sf::Vector2f sepVector = factor*(GJKResult.v2 - GJKResult.v1);
            tempObject.setVelocity(dynamicObjects[i]->getVelocity() - sepVector);

            Arbiter tempArb{dynamicObjects[i].get(), &tempObject};
            Contact tempContact;
            tempContact.normal = sfVectorMath::norm(GJKResult.v1 - GJKResult.v2);
            tempContact.position = GJKResult.v1;
            tempContact.rA = tempContact.position - dynamicObjects[i]->getPosition();
            tempContact.rB = {0.0f, 0.0f};
            tempArb.contacts.push_back(tempContact);

            tempArb.PreStep(1.0f/dt);
            tempArb.ApplyImpulse();

            dynamicObjects[i]->addSolvedVelocity(factor*tempContact.normal,
                                                 factor*tempContact.normal);
        }
    }
}

void BallUniverse::newJoint(int index1, int index2)
{
    if(index1 < dynamicObjects.size() &&
       index2 < dynamicObjects.size() &&
       index1 != index2)
    {
        //std::cout << "joint created\n";
        //Joint nJoint(dynamicObjects[index1].get(),
        //               dynamicObjects[index2].get());
        //joints.push_back(nJoint);
        int objSize = dynamicObjects.size();
        Joint nJoint(dynamicObjects[objSize-2].get(),
                       dynamicObjects[objSize-1].get());
        joints.push_back(nJoint);
    }
}

void BallUniverse::newObserver(Observer* obs)
{
    universeSub.addObserver(obs);
}

BallUniverse::BallUniverse(int _worldSizeX,
                           int _worldSizeY,
                           float _dt,
                           bool _force,
                           bool _collision) :

worldSizeX{_worldSizeX},
worldSizeY{_worldSizeY},
enable_forces{_force},
enable_collisions{_collision},
dt{_dt}
{
}
