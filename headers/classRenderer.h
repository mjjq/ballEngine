#ifndef CLASS_RENDERER_H
#define CLASS_RENDERER_H

#include <vector>
#include "SFML/Graphics.hpp"
#include "classRenderable.h"
#include "Observer.h"

class Renderer : public Observer
{
    std::vector<Renderable* > renderObjects;
    std::map<std::string, sf::Texture> loadedTextures;
public:
    Renderer();
    void redrawAll(sf::RenderWindow& window);
    void onNotify(Entity& entity, Event event);
};

#endif // CLASS_RENDERER_H
