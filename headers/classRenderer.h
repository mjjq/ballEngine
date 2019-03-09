#ifndef CLASS_RENDERER_H
#define CLASS_RENDERER_H

#include <vector>
#include "SFML/Graphics.hpp"
#include "classRenderable.h"
#include "Observer.h"

class Renderer : public Observer
{
    const std::string TEXTURE_PATH = "./res/textures/";
    std::vector<Renderable* > renderObjects;
    std::map<std::string, sf::Texture> loadedTextures;

    bool loadTexture(std::string textureName);
    bool textureIsLoaded(std::string textureName);
public:
    Renderer();
    void redrawAll(sf::RenderWindow& window);
    void onNotify(Entity& entity, Event event);
};

#endif // CLASS_RENDERER_H
