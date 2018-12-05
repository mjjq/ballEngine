#include "SpawnInterface.h"

void SpawnInterface::getWorldAddress(BallUniverse* address)
{
    worldAddress = address;
}

void spawnNewObject(bool isStatic,
                    SpawnObjectType type,
                    ObjectProperties init)
{
    /*sf::Vector2f position = init._position;
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
    }*/
}
