#ifndef CLASS_RENDERER_H
#define CLASS_RENDERER_H

#include <vector>
#include "SFML/Graphics.hpp"
#include "lightingEngine.h"
#include "classRenderable.h"
#include "classLight.h"
#include "Observer.h"
#include "windowManager.h"

class Renderer : public Observer
{
    const std::string TEXTURE_PATH = "./res/textures/";
    const std::string SHADER_PATH = "./res/shaders/";
    const std::string FRAGMENT_EXTENSION = ".frag";
    const std::string VERT_EXTENSION = ".vert";
    const std::string GEOMETRY_EXTENSION = ".geom";

    std::vector<Renderable* > renderObjects;
    std::map<std::string, sf::Texture> loadedTextures;
    std::map<std::string, sf::Shader > loadedShaders;

    LightingEngine lightingEngine;

    bool loadTexture(std::string textureName);
    bool loadShader(std::string shaderName);
    bool textureIsLoaded(std::string textureName);
    bool shaderIsLoaded(std::string shaderName);
public:
    WindowManager windowManager;

    Renderer();
    void redrawAll(sf::RenderWindow& window);
    void onNotify(Entity& entity, Event event);
    void resizeWindow(sf::Vector2u newSize);
};

#endif // CLASS_RENDERER_H
