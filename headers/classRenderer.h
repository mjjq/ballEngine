#ifndef CLASS_RENDERER_H
#define CLASS_RENDERER_H

#include <vector>
#include "SFML/Graphics.hpp"
#include "lightingEngine.h"
#include "classRenderable.h"
#include "classLight.h"
#include "Observer.h"
#include "windowManager.h"
#include "resourceManager.h"

class Renderer : public Observer
{
    std::vector<Renderable* > renderObjects;

    LightingEngine lightingEngine;
    ResourceManager resourceManager;
public:
    WindowManager windowManager;

    Renderer();
    void redrawAll(sf::RenderWindow& window);
    void onNotify(Component& entity, Event event, Container* data = nullptr);
    void resizeWindow(sf::Vector2u newSize);
};

#endif // CLASS_RENDERER_H
