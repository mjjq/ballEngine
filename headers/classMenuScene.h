#ifndef CLASS_MENUSCENE_H
#define CLASS_MENUSCENE_H


#include "classScene.h"
#include "classUIContainer.h"

class MenuScene : public Scene
{
    sf::RenderWindow &window;
    sf::View worldView = window.getDefaultView();
    sf::View GUIView = window.getDefaultView();
    float currentZoom = 1.0f;

    sf::Time &currentFrameTime;
    float &currentFPS;

    sf::Vector2i mousePosOnClick;
    sf::Vector2i mousePosOnPan;
    sf::Vector2i mousePosOnRelease;

    std::pair<bool,int> mouseOnUIWhenClicked{false, -1};
    bool clickedWindowToDrag = false;

    UIContainer container{true};

    void resetView();
    void adjustViewSize(sf::Vector2u newSize);

    void checkMBPress(sf::Vector2i &initPos, bool mouseType);
    void resetUIClick();
    void clickOnUI();

    void mouseViewEvents(sf::Event &event);
    void mouseUIEvents(sf::Event &event);
    void resizeEvents(sf::Event &event);

public:
    MenuScene(sf::RenderWindow &window,
              sf::Time &currentFTime, float &currentFPS);
    void update(sf::RenderWindow &window);
    void load();
    void events(sf::Event &event);
    void redraw(sf::RenderWindow &window);
    void unload();

};



#endif
