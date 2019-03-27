#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "classRenderable.h"

class ResourceManager
{
    const std::string TEXTURE_PATH = "./res/textures/";
    const std::string SHADER_PATH = "./res/shaders/";
    const std::string FRAGMENT_EXTENSION = ".frag";
    const std::string VERT_EXTENSION = ".vert";
    const std::string GEOMETRY_EXTENSION = ".geom";

    std::map<std::string, sf::Texture> loadedTextures;
    std::map<std::string, sf::Shader > loadedShaders;
public:
    bool loadTexture(std::string const & textureName);
    bool textureIsLoaded(std::string const & textureName);
    bool loadShader(std::string const & shaderName);
    bool shaderIsLoaded(std::string const & shaderName);

    sf::Shader* getShader(std::string const & shaderName);
    sf::Texture* getTexture(std::string const & textureName);

    void assignResources(Renderable& object);
};

#endif // RESOURCE_MANAGER_H
