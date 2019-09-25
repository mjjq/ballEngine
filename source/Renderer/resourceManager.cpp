#include <iostream>

#include "resourceManager.h"

bool ResourceManager::loadTexture(std::string const & textureName)
{
    std::string fullFilePath = TEXTURE_PATH + textureName;

    if(textureName.length() < 4)
        return false;

    if(textureName.at(0) == '.' || textureName.at(0) == '/')
        fullFilePath = textureName;

    if(loadedTextures.find(textureName) == loadedTextures.end() &&
       textureName != Renderable::NULL_ID)
    {
        sf::Texture texture;
        if(texture.loadFromFile(fullFilePath))
        {
            loadedTextures.insert({textureName, texture});
            return true;
        }
    }

    std::cout << "failed to load texture: " << textureName << "\n";
    return false;
}

bool ResourceManager::textureIsLoaded(std::string const & textureName)
{
    if(loadedTextures.find(textureName) != loadedTextures.end() &&
       textureName != Renderable::NULL_ID)
        return true;

    //std::cout << "texture: " << textureName << " isn't loaded\n";
    return false;
}

bool ResourceManager::loadShader(std::string const & shaderName)
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

bool ResourceManager::shaderIsLoaded(std::string const & shaderName)
{
    if(loadedShaders.find(shaderName) != loadedShaders.end())
        return true;

    return false;
}


sf::Shader* ResourceManager::getShader(std::string const & shaderName)
{
    if(shaderIsLoaded(shaderName))
        return &loadedShaders[shaderName];

    return nullptr;
}


sf::Texture* ResourceManager::getTexture(std::string const & textureName)
{
    if(textureIsLoaded(textureName))
        return &loadedTextures[textureName];

    return nullptr;
}


void ResourceManager::assignResources(Renderable& object)
{
    if(object.primShape != nullptr)
    {
        if(loadTexture(object.material.diffuseID) ||
                        textureIsLoaded(object.material.diffuseID))
        {
            std::cout << "assigned texture\n";
            object.primShape->setTexture(&loadedTextures[object.material.diffuseID]);
            object.primShape->setFillColor(sf::Color::White);
            object.primShape->setOutlineThickness(0);
        }
    }
    if(loadShader(object.material.shaderID) ||
                    shaderIsLoaded(object.material.shaderID))
    {
        object.shader = &loadedShaders[object.material.shaderID];
    }
    loadTexture(object.material.normalID);
    loadTexture(object.material.emissionID);
}
