#ifndef CLASS_PAUSEMENUSCENE_H
#define CLASS_PAUSEMENUSCENE_H


#include "classScene.h"
#include "classUIContainer.h"

class PauseMenuScene : public Scene
{
public:
    PauseMenuScene(sf::RenderWindow &window);
    void update(sf::RenderWindow &window);
    void load();
    void events(sf::Event &event);
    void redraw(sf::RenderWindow &window);
    void unload();

};



#endif
