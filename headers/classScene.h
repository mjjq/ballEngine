#ifndef CLASS_SCENE_H
#define CLASS_SCENE_H

#include "classUIContainer.h"
#include "classKeyBinds.h"
#include "sceneEnums.h"
#include <fstream>

class Scene
{
    SceneEnum nextScene = SceneEnum::LAST;

protected:
    SceneEnum prevScene = SceneEnum::LAST;

    StringStringMap stringKeyBinds;
    StringFuncMap buttonFuncMap = {};
    std::map<std::string, std::pair<std::function<void(float)>, float*> > sliderFuncMap = {};
    std::map<std::string, std::function<std::string()> > textVarMap = {};

    KeyFuncMap keyBinds = {};
    std::vector<sf::Keyboard::Key > pressedKeyStack = {};

    sf::RenderWindow &window;
    sf::View worldView = window.getDefaultView();
    sf::View GUIView = window.getDefaultView();

    UIContainer container{true};
    std::pair<bool,int> mouseOnUIWhenClicked{false, -1};
    bool clickedWindowToDrag = false;

    virtual void resetUIClick();
    virtual void clickOnUI();

    virtual void mouseViewEvents(sf::Event &event);
    virtual void mouseUIEvents(sf::Event &event);
    virtual void resizeEvents(sf::Event &event);

public:
    Scene(sf::RenderWindow &_window);

    void requestScene(SceneEnum scEnum);
    SceneEnum pollNextScene();
    void setPrevScene(SceneEnum scEnum);

    virtual void update(sf::RenderWindow &window);
    virtual void redraw(sf::RenderWindow &window);
    virtual void initBindings();

    void loadUI(std::string filePath, UIContainer &container);
    void loadKeybinds(std::string filePath,
                      std::string sceneType);

    virtual void load();
    virtual void unload();
    virtual void adjustViewSize(sf::Vector2u newSize);
    virtual void events(sf::Event &event);

    void keyEvents(sf::Event &event);
    void exePressedKeys();
};

#endif // CLASS_SCENE_H
