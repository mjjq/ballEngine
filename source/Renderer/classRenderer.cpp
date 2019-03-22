#include "classRenderer.h"
#include <iostream>
#include <cmath>
#include "sfVectorMath.h"
#include <SFML/OpenGL.hpp>

Renderer::Renderer()
{
    Renderable::renderSubject.addObserver(this);
    LightSource::renderSubject.addObserver(this);
}

bool Renderer::loadTexture(std::string textureName)
{
    if(loadedTextures.find(textureName) == loadedTextures.end() &&
       textureName != Renderable::NULL_ID)
    {
        sf::Texture texture;
        if(texture.loadFromFile(TEXTURE_PATH + textureName))
        {
            loadedTextures.insert({textureName, texture});
            return true;
        }
    }

    std::cout << "failed to load texture: " << textureName << "\n";
    return false;
}

bool Renderer::textureIsLoaded(std::string textureName)
{
    if(loadedTextures.find(textureName) != loadedTextures.end() &&
       textureName != Renderable::NULL_ID)
        return true;

    std::cout << "texture isn't loaded\n";
    return false;
}

void Renderer::redrawAll(sf::RenderWindow &window)
{
    clearShadowTextures();
    generateShadowTextures(renderObjects);
    displayShadowTextures();
    for(int i=0; i<(int)renderObjects.size(); ++i)
    {
        if(renderObjects[i]->shader != nullptr)
        {
            sf::Shader* shader = renderObjects[i]->shader;
            for(int j=0; j<10; ++j)
            {
                std::string lightVal = "lights[" + std::to_string(j) + "]";
                std::string shadowVal = "shadowTextures[" + std::to_string(j) + "]";
                if(j < (int)lights.size())
                {
                    shader->setUniform("light.color", lights[j]->lightProperties.color);
                    shader->setUniform(lightVal + ".color", lights[j]->lightProperties.color);
                    shader->setUniform(lightVal + ".position", lights[j]->position);
                    shader->setUniform(lightVal + ".constant", lights[j]->lightProperties.constant);
                    shader->setUniform(lightVal + ".linear", lights[j]->lightProperties.linear);
                    shader->setUniform(lightVal + ".quadratic", lights[j]->lightProperties.quadratic);
                    //shader->setUniform(shadowVal, &shadowTextures[j]->getTexture());
                }
                else
                {
                    shader->setUniform(lightVal + ".color", sf::Vector3f(0.0,0.0,0.0));
                    shader->setUniform(lightVal + ".position", sf::Vector3f(0.0,0.0,0.0));
                    shader->setUniform(lightVal + ".constant", 1.0f);
                    shader->setUniform(lightVal + ".linear", 0.0f);
                    shader->setUniform(lightVal + ".quadratic", 0.0f);
                    //shader->setUniform(shadowVal, &blankShadowTexture);
                }
            }
            if(shadowTextures.size() > 0)
                shader->setUniform("shadowTexture", shadowTextures[0]->getTexture());

            float rotation = sfVectorMath::PI * renderObjects[i]->primTransformable->getRotation() / 180.0f;
            shader->setUniform("rotCosine", (float)cos(rotation));
            shader->setUniform("rotSine", (float)sin(rotation));

            shader->setUniform("material.diffuseMap", loadedTextures[renderObjects[i]->material.diffuseID]);
            shader->setUniform("material.normalMap", loadedTextures[renderObjects[i]->material.normalID]);
            shader->setUniform("material.emissionMap", loadedTextures[renderObjects[i]->material.emissionID]);
            shader->setUniform("material.diffuseStrength", renderObjects[i]->material.diffuseStrength);
            shader->setUniform("material.ambientStrength", renderObjects[i]->material.ambientStrength);
            shader->setUniform("material.specularStrength", renderObjects[i]->material.specularStrength);
            shader->setUniform("material.emissionStrength", renderObjects[i]->material.emissionStrength);
            shader->setUniform("material.shininess", renderObjects[i]->material.shininess);
        }

        window.draw(*renderObjects[i]->primDrawable, renderObjects[i]->shader);
    }

}

void Renderer::onNotify(Entity& entity, Event event)
{
    switch(event.type)
    {
        case(EventType::New_Renderable):
        {
            Renderable* ren = (Renderable*)&entity;
            if(ren->primShape != nullptr)
            {
                if(loadTexture(ren->material.diffuseID) || textureIsLoaded(ren->material.diffuseID))
                {
                    std::cout << "assigned texture\n";
                    ren->primShape->setTexture(&loadedTextures[ren->material.diffuseID]);
                    ren->primShape->setFillColor(sf::Color::White);
                    ren->primShape->setOutlineThickness(0);
                }
            }
            if(loadShader(ren->material.shaderID) || shaderIsLoaded(ren->material.shaderID))
            {
                ren->shader = &loadedShaders[ren->material.shaderID];
            }
            loadTexture(ren->material.normalID);
            loadTexture(ren->material.emissionID);

            renderObjects.push_back(ren);
            break;
        }
        case(EventType::Delete_Renderable):
        {
            Renderable* obj = (Renderable*)&entity;
            for(int i=0; i<(int)renderObjects.size(); ++i)
                if(renderObjects[i] == obj)
                    renderObjects.erase(renderObjects.begin() + i);
            break;
        }
        case(EventType::New_LightSrc):
        {
            LightSource* lSrc = (LightSource*)&entity;
            lights.push_back(lSrc);

            sf::Vector2u winSize = windowManager.getWindowSize();
            sf::RenderTexture* texture = new sf::RenderTexture;
            texture->create(winSize.x, winSize.y);
            shadowTextures.push_back(std::move(texture));
            break;
        }
        case(EventType::Delete_LightSrc):
        {
            LightSource* obj = (LightSource*)&entity;
            for(int i=0; i<(int)lights.size(); ++i)
                if(lights[i] == obj)
                {
                    lights.erase(lights.begin() + i);

                    sf::RenderTexture* tToDelete = shadowTextures[i];
                    shadowTextures.erase(shadowTextures.begin() + i);
                    delete tToDelete;
                }
            break;
        }
        default:
            break;
    }
}

bool Renderer::loadShader(std::string shaderName)
{
    std::string filePath = SHADER_PATH + shaderName;

    if(sf::Shader::isAvailable() &&
       !shaderIsLoaded(shaderName) &&
       shaderName != Renderable::NULL_ID)
    {
        bool isFrag = false;
        bool isGeom = false;
        bool isVert = false;

        if(shaderName.find(FRAGMENT_EXTENSION) != std::string::npos)
            isFrag = true;
        else if(shaderName.find(GEOMETRY_EXTENSION) != std::string::npos)
            isGeom = true;
        else if(shaderName.find(VERT_EXTENSION) != std::string::npos)
            isVert = true;

        if(isFrag)
        {
            if(loadedShaders[shaderName].loadFromFile(filePath, sf::Shader::Fragment))
                return true;
        }
        else if(isGeom)
        {
            if(loadedShaders[shaderName].loadFromFile(filePath, sf::Shader::Geometry))
                return true;
        }
        else if(isVert)
        {
            if(loadedShaders[shaderName].loadFromFile(filePath, sf::Shader::Vertex))
                return true;
        }
        else
        {
            std::string vertName = filePath + VERT_EXTENSION;
            std::string fragName = filePath + FRAGMENT_EXTENSION;
            std::string geomName = filePath + GEOMETRY_EXTENSION;

            if(sf::Shader::isGeometryAvailable())
            {
                if(loadedShaders[shaderName].loadFromFile(vertName, geomName, fragName))
                    return true;
            }
            else
                std::cout << "geometry shaders unavailable\n";
            if(loadedShaders[shaderName].loadFromFile(vertName, fragName))
                return true;
        }
    }
    std::cout << "failed to load shaders: " << shaderName << "\n";
    return false;
}

bool Renderer::shaderIsLoaded(std::string shaderName)
{
    if(loadedShaders.find(shaderName) != loadedShaders.end())
        return true;

    return false;
}


void Renderer::clearShadowTextures()
{
    sf::Vector2u winSize = windowManager.getWindowSize();
    blankShadowTexture.create(winSize.x, winSize.y);
    for(int i=0; i<(int)shadowTextures.size(); ++i)
    {
        shadowTextures[i]->setView(windowManager.getWindow().getView());
        shadowTextures[i]->clear(sf::Color::White);
    }
}

void Renderer::displayShadowTextures()
{
    for(int i=0; i<(int)shadowTextures.size(); ++i)
        shadowTextures[i]->display();
}

void Renderer::generateShadowTextures(std::vector<Renderable* > const & _renderObjects)
{
    for(int i=0; i<(int)lights.size(); ++i)
    {
        for(int j=0; j<(int)_renderObjects.size(); ++j)
        {

            lights[i]->shadowStencil(*renderObjects[j]->primShape, *shadowTextures[i]);

            std::string shadowTextureString = "shadowTextures[" + std::to_string(i) + "]";
            if(_renderObjects[j]->shader != nullptr)
                _renderObjects[j]->shader->setUniform(shadowTextureString,
                                                      shadowTextures[i]->getTexture());
        }
    }

    /*if(shadowTextures.size()>0)
    {
        sf::Sprite shape(shadowTextures[0]->getTexture());
        windowManager.getWindow().draw(shape);
    }*/
}

void Renderer::resizeWindow(sf::Vector2u newSize)
{
    for(int i=0; i<(int)shadowTextures.size(); ++i)
    {
        shadowTextures[i]->create(newSize.x, newSize.y);
    }
    clearShadowTextures();
}
