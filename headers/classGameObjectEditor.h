#ifndef CLASS_GOEDITOR_H
#define CLASS_GOEDITOR_H

#include "classGameObject.h"
#include "classGameObjectManager.h"
#include "classJoint.h"

class GameObjectEditor
{
    GameObjectManager& gameObjectManager;
    sf::RenderWindow& viewWindow;
    GameObject* currentObject = nullptr;
    SocketJointSingle* grabber = nullptr;

    sf::RenderTexture selectionTexture;
    sf::Shader selectionShader;
public:
    GameObjectEditor(GameObjectManager& manager,
                     sf::RenderWindow& window);
    void selectObject(sf::Vector2u const & position);
    void releaseObject();
    void deleteObject();

    template <typename T>
    void setObjectAttribute(std::string const & attribute,
                            T const & attributeValue)
    {
        if(currentObject != nullptr)
        {
            if(attribute == "position")
                currentObject->setPosition(attributeValue);
            else if(attribute == "velocity")
                currentObject->setVelocity(attributeValue);
            else
                std::cout << "Object has no attribute " << attribute << "\n";
        }
    }

    void ungrabObject()
    {
        if(grabber != nullptr)
        {
            delete grabber;
            grabber = nullptr;
        }
    }

    void grabObject(std::function<sf::Vector2f() > grabPosition)
    {
        ungrabObject();
        if(currentObject != nullptr)
        {
            grabber = new SocketJointSingle({currentObject->getColliderAddress()}, grabPosition);
        }
    }

};

#endif // CLASS_GOEDITOR_H
