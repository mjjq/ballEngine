#ifndef CLASS_RENDERER_H
#define CLASS_RENDERER_H

#include <vector>
#include "SFML/Graphics.hpp"
#include "classRenderable.h"
#include "classLight.h"
#include "Observer.h"

class Renderer : public Observer
{
    const std::string TEXTURE_PATH = "./res/textures/";
    const std::string SHADER_PATH = "./res/shaders/";
    const std::string FRAGMENT_EXTENSION = ".frag";
    const std::string VERT_EXTENSION = ".vert";

    std::vector<Renderable* > renderObjects;
    std::vector<LightSource* > lights;
    std::map<std::string, sf::Texture> loadedTextures;
    std::map<std::string, sf::Shader > loadedShaders;

    bool loadTexture(std::string textureName);
    bool loadShader(std::string shaderName);
    bool textureIsLoaded(std::string textureName);
    bool shaderIsLoaded(std::string shaderName);
public:
    Renderer();
    void redrawAll(sf::RenderWindow& window);
    void onNotify(Entity& entity, Event event);
};

#endif // CLASS_RENDERER_H
