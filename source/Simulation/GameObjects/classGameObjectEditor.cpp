#include "classGameObjectEditor.h"

GameObjectEditor::GameObjectEditor(GameObjectManager& manager,
                                    sf::RenderWindow& window) :
    gameObjectManager{manager}, viewWindow{window}
{
    const std::string indShader = \
        "uniform float color;" \
        "void main()" \
        "{" \
        "   gl_FragColor = vec4(color, 0.0, 0.0, 1.0);" \
        "}";

    if(!selectionShader.loadFromMemory(indShader, sf::Shader::Fragment))
    {
        std::cout << "Editor: failed to load shader\n";
    }
}

void GameObjectEditor::selectObject(sf::Vector2u const & position)
{
    selectionTexture.create(viewWindow.getSize().x, viewWindow.getSize().y);
    selectionTexture.setView(viewWindow.getView());
    selectionTexture.display();
    selectionTexture.clear({0, 0, 0, 0});

    int numObjects = gameObjectManager.gameObjects.size();
    sf::RenderStates drawState(&selectionShader);

    for(int i=0; i<numObjects; ++i)
    {
        Renderable* renderObj = gameObjectManager.gameObjects[i]->renderObj;
        float color = (float)((float)i/(float)numObjects);
        selectionShader.setUniform("color", color);

        if(renderObj != nullptr)
        {
            drawState.transform = renderObj->getTransform();
            selectionTexture.draw(renderObj->verts,
                                  drawState);
        }
    }

    sf::Image finalImage = selectionTexture.getTexture().copyToImage();
    sf::Color pixelColor = finalImage.getPixel(position.x, position.y);

    if(pixelColor.a != 0)
    {
        int objectIndex = round((float)pixelColor.r *
                                (float)gameObjectManager.gameObjects.size() /
                                (float)255);
        currentObject = gameObjectManager.gameObjects[objectIndex];
    }
}

void GameObjectEditor::releaseObject()
{
    currentObject = nullptr;
}

void GameObjectEditor::deleteObject()
{
    if(currentObject != nullptr)
    {
        //gameObjectManager.removeObject(currentObject);
        delete currentObject;
        releaseObject();
    }
    else
        releaseObject();
}
