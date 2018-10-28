#ifndef CLASS_MENUSCENE_H
#define CLASS_MENUSCENE_H


#include "classScene.h"
#include "classUIContainer.h"

class MenuScene : public Scene
{

public:
    MenuScene(sf::RenderWindow &window);
    void update(sf::RenderWindow &window);
    void load();
    void events(sf::Event &event);
    void redraw(sf::RenderWindow &window);
    void unload();

};



#endif
