#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>

#include "classes.h"
#include "sfVectorMath.h"


void BallUniverse::debugStuff()
{
    std::string mouse_x{(sf::Mouse::getPosition(window).x)};
    mouse_x += " " + (sf::Mouse::getPosition(window).y);
    mousePos.setString(mouse_x);
    mousePos.setColor(sf::Color::Red);
    window.draw(mousePos);
}

void BallUniverse::spawnNewBall(sf::Vector2f position, sf::Vector2f velocity, float radius, float mass)
{
    if(!(position.x - radius < 0 || position.y - radius < 0
      || position.x + radius > worldSizeX || position.y + radius > worldSizeY))
        ballArray.push_back(Ball(radius,mass,position,velocity));
    /*std::tuple<int,int,float> collTuple = findShortestCollTime(timeToNextColl, ballArray);
    timeToNextColl = std::get<2>(collTuple);
    collider1 = std::get<0>(collTuple);
    collider2 = std::get<1>(collTuple);
    currentTime = 0;*/
   // std::cout << ballArray.size() << "\n";
}

//void resetAndCheckBounce(std::vector<Ball>)

std::tuple<int,int,float> BallUniverse::findShortestCollTime(float t_coll, std::vector<Ball> &ballArray, float dt)
{
    int coll1 = 0; //collide function won't execute if collider1 = collider2 as this wouldn't make sense physically
    int coll2 = 0;
    float localT_coll = t_coll;

    for(int i=0;i<ballArray.size();i++)
    {
        for(int j=0;j<ballArray.size();j++)
        {
            if(i!=j)
            {
                /*if(ballArray.at(i).getDistance(ballArray.at(j)) < 0.8*(ballArray.at(i).getRadius() + ballArray.at(j).getRadius()))
                    std::cout << "Overlapping balls at: " << ballArray.at(i).getPosition().x << "\n";*/
                //if(ballArray.at(i).getHasCollided()==false && ballArray.at(j).getHasCollided()==false)

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
                        ballArray.at(i).setToCollided();
                        ballArray.at(j).setToCollided();

                    }
                }

            }
        }
    }
    //std::cout << localT_coll << "\n";
    return std::make_tuple(coll1,coll2,localT_coll);
}

void BallUniverse::universeLoop(float dt)
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
            ballArray.at(collider1).ballCollision(ballArray.at(collider2));
            ballArray.at(collider1).resetToCollided();
            ballArray.at(collider2).resetToCollided();
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
            ballArray.at(i).resetToCollided();
        }
    }
    for(int i=0; i<ballArray.size(); i++)
        window.draw(ballArray.at(i));

}

void BallUniverse::universeLoopNoForces(float dt)
{
    //int collider1 = std::numeric_limits<int>::quiet_NaN();
    //int collider2 = std::numeric_limits<int>::quiet_NaN();
    float dtR = dt;
    float epsilon = 1e-5;

    for(int i=0;i<ballArray.size();i++)
    {
        ballArray.at(i).resetToCollided();
        ballArray.at(i).checkForBounce(windowSizeX,windowSizeY);
    }

    if(dt + currentTime > timeToNextColl)
    {
        //std::cout << "next collision: " <<timeToNextColl << "\n";
        dtR = timeToNextColl - currentTime;
        //std::cout << dtR << "\n";
        //std::cout << std::floor(1e+5*dtR)/1e+5 << "\n";
        if(std::abs(dtR) > epsilon)
            for(int i=0;i<ballArray.size();i++)
            {
                ballArray.at(i).updatePosition(std::floor(1e+5*dtR)/1e+5);
                window.draw(ballArray.at(i));
            }
        if(collider1 != collider2)
        {
            ballArray.at(collider1).ballCollision(ballArray.at(collider2));
            std::cout << "Collision at ";
            sfVectorMath::printVector(ballArray.at(collider1).getPosition());
            std::cout << "\n";
            //ballArray.at(collider1).resetToCollided();
            //ballArray.at(collider2).resetToCollided();
            //std::cout << getTotalKE(ballArray) <<"\n";
        }
        timeToNextColl = 1e+15;

        std::tuple<int,int,float> collTuple = findShortestCollTime(timeToNextColl, ballArray);
        timeToNextColl = std::get<2>(collTuple);
        collider1 = std::get<0>(collTuple);
        collider2 = std::get<1>(collTuple);

        currentTime = -dtR;
    }
    else
    {
        //std::cout <<"no collide";
        for(int i=0;i<ballArray.size();i++)
        {
            ballArray.at(i).updatePosition(dt);
            window.draw(ballArray.at(i));
            ballArray.at(i).resetToCollided();
        }
    }
    currentTime += dtR;
    //std::cout << currentTime << "\n";

}

void BallUniverse::checkMBPress(sf::Vector2i &initPos, bool mouseType)
{
    sf::Vector2i finalPos;
    sf::Vertex line[2];

    if((mouseType == true))
    {
        sf::Vector2i viewPos = sf::Mouse::getPosition(window);
        finalPos = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));
        //finalPos = sf::Mouse::getPosition(window);
        line[0] = sf::Vertex(static_cast<sf::Vector2f>(initPos),sf::Color::White);
        line[1] = sf::Vertex(static_cast<sf::Vector2f>(finalPos),sf::Color::White);
        window.draw(line, 2, sf::Lines);
    }
}

float BallUniverse::getTotalKE(std::vector<Ball> &ballArray)
{
    float totalKE{0};
    for(int i=0; i<ballArray.size(); i++)
        totalKE += ballArray.at(i).getKE();
    return totalKE;
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

sf::Vector2f BallUniverse::velocityFromMouse(sf::Vector2i mousePosOnClick)
{

    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
    sf::Vector2i mousePosOnRelease = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));
    //sf::Vector2i mousePosOnRelease = sf::Mouse::getPosition(window);
    sf::Vector2i scaledVector = (spawnVelFactor)*(mousePosOnRelease-mousePosOnClick);
    float windowDiagSize = pow(windowSizeX*windowSizeX + windowSizeY*windowSizeY, 0.5);
    sf::Vector2f velocity = static_cast<sf::Vector2f>(scaledVector)/windowDiagSize;
    timeToNextSpawn = minTimeToNextSpawn;

    return velocity;
}

void BallUniverse::zoomToMouse(float zoomFactor)
{

    currentZoom *= zoomFactor;
    worldView.zoom(zoomFactor);
    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(viewPos);
    worldView.setCenter(mousePos);
    window.setView(worldView);
}

sf::Vector2f BallUniverse::getEffectiveZoom()
{
    float zoom = currentZoom;
    float winSizeX = window.getSize().x;
    float winSizeY = window.getSize().y;
    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
    float xFactor = zoom*static_cast<float>(worldSizeX)/static_cast<float>(winSizeX);
    float yFactor = zoom*static_cast<float>(worldSizeY)/static_cast<float>(winSizeY);
    sf::Vector2f effectiveZoom = {xFactor,yFactor};
    return effectiveZoom;
}

void BallUniverse::checkForViewPan(sf::Vector2i initialPos, sf::Vector2f originalView, bool keyBool)
{
    if(keyBool==true)
    {
        window.setMouseCursorVisible(false);
        sf::Vector2f effectiveZoom = getEffectiveZoom();
        sf::Vector2i viewPos = sf::Mouse::getPosition(window);
        sf::Vector2f relPos = static_cast<sf::Vector2f>(initialPos - viewPos);
        //sfVectorMath::printVector(viewPos);
        //std::cout <<"\n";
        if(effectiveZoom.y > effectiveZoom.x)
            relPos *= effectiveZoom.y;
        else
            relPos *= effectiveZoom.x;


        worldView.setCenter(relPos + originalView);

        window.setView(worldView);
    }
}

void BallUniverse::adjustViewSize(int sizeX, int sizeY, float zoom)
{
    float xFactor = zoom*static_cast<float>(worldSizeX)/static_cast<float>(sizeX);
    float yFactor = zoom*static_cast<float>(worldSizeY)/static_cast<float>(sizeY);

    worldView.setSize(sizeX,sizeY);
    windowSizeX = sizeX;
    windowSizeY = sizeY;

    if(yFactor > xFactor)
        worldView.zoom(yFactor);
    else
        worldView.zoom(xFactor);

    window.setView(worldView);
}


BallUniverse::BallUniverse(int m_windowSizeX, int m_windowSizeY, int worldSizeX, int worldSizeY, float dt, int spawnVelFactor,
                            float spawnRadius, float spawnMass, float ballGridSpacing, int ballGridHeight,
                            int ballGridWidth, bool force, bool collision) :

windowSizeX(m_windowSizeX), windowSizeY{m_windowSizeY}, worldSizeX{worldSizeX}, worldSizeY{worldSizeY}, enable_forces(force),
enable_collisions(collision), dt{dt}, spawnVelFactor{spawnVelFactor},
spawnRadius{spawnRadius}, spawnMass{spawnMass}, ballGridSpacing{ballGridSpacing},
ballGridHeight{ballGridHeight}, ballGridWidth{ballGridWidth}
{
    currentZoom = 1.0;
    worldView.setCenter(worldSizeX/2,worldSizeY/2);
    adjustViewSize(window.getSize().x, window.getSize().y, currentZoom);
    window.setView(worldView);
}

void BallUniverse::universeMainLoop()
{
    sf::Vector2i mousePosOnClick;
    sf::Vector2i mousePosOnRelease;
    sf::Vector2f recentViewCoords;

    while(window.isOpen())
    {
        window.clear();
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i viewPos = sf::Mouse::getPosition(window);
                mousePosOnClick = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));
            }
            window.setKeyRepeatEnabled(false);
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                //std::cout << "thing\n";
                recentViewCoords = worldView.getCenter();
                mousePosOnClick = sf::Mouse::getPosition(window);
                //mousePosOnClick = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));
            }
            //window.setKeyRepeatEnabled(true);

            if(event.type == sf::Event::EventType::MouseButtonReleased
                            && event.mouseButton.button==sf::Mouse::Left
                            && !(timeToNextSpawn > sf::milliseconds(0)))
            {
                sf::Vector2f velocity = velocityFromMouse(mousePosOnClick);
                spawnNewBall(static_cast<sf::Vector2f>(mousePosOnClick),velocity,spawnRadius,spawnMass);
            }

            /*if(event.type == sf::Event::EventType::MouseButtonReleased
                            && event.mouseButton.button==sf::Mouse::Right
                            && !(timeToNextSpawn > sf::milliseconds(0)))
            {
                sf::Vector2f velocity = velocityFromMouse(mousePosOnClick);
                spawnNewBall(static_cast<sf::Vector2f>(mousePosOnClick),velocity,spawnRadius,-spawnMass);
            }*/

            if(event.type == sf::Event::EventType::MouseButtonReleased
                            && event.mouseButton.button==sf::Mouse::Middle
                            && !(timeToNextSpawn > sf::milliseconds(0)))
            {
                sf::Vector2f velocity = velocityFromMouse(mousePosOnClick);
                createBallGrid(ballGridWidth,ballGridHeight,ballGridSpacing,
                                static_cast<sf::Vector2f>(mousePosOnClick),velocity,spawnMass,spawnRadius);
            }

            if(event.type == sf::Event::Resized)
                adjustViewSize(event.size.width,event.size.height, currentZoom);

            if(event.type == sf::Event::EventType::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::PageUp)
                    timestep+=sf::milliseconds(10);
                else if(event.key.code == sf::Keyboard::PageDown && timestep>sf::milliseconds(15))
                    timestep-=sf::milliseconds(10);
                else if(event.key.code == sf::Keyboard::Left)
                    dt+=1;
                else if(event.key.code == sf::Keyboard::Right && dt>=2)
                    dt-=1;
                else if(event.key.code == sf::Keyboard::Delete)
                    ballArray.clear();
                else if(event.key.code == sf::Keyboard::Dash)
                    zoomToMouse(2.0f);
                else if(event.key.code == sf::Keyboard::Equal)
                    zoomToMouse(0.5f);
                else if(event.key.code == sf::Keyboard::Num0)
                {
                    currentZoom = 1.0;
                    worldView.setCenter(worldSizeX/2,worldSizeY/2);
                    adjustViewSize(window.getSize().x, window.getSize().y, currentZoom);
                }

                /*else if(event.key.code == sf::Keyboard::Space)
                {
                    std::cout << "thing\n";
                    sf::Vector2i viewPos = sf::Mouse::getPosition(window);
                    mousePosOnClick = static_cast<sf::Vector2i>(window.mapPixelToCoords(viewPos));
                }*/
            }
        }
        if(event.type == sf::Event::EventType::KeyReleased)
        {
            if(event.key.code == sf::Keyboard::Space)
                window.setMouseCursorVisible(true);
        }

        checkMBPress(mousePosOnClick,sf::Mouse::isButtonPressed(sf::Mouse::Left));
        checkMBPress(mousePosOnClick,sf::Mouse::isButtonPressed(sf::Mouse::Middle));
        checkMBPress(mousePosOnClick,sf::Mouse::isButtonPressed(sf::Mouse::Right));
        checkForViewPan(mousePosOnClick,recentViewCoords, sf::Keyboard::isKeyPressed(sf::Keyboard::Space));

        universeLoop(dt);

        window.display();
        sf::sleep(timestep);
        timeToNextSpawn -= timestep;
    }
}

