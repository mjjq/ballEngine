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
    StringStringMap relStrKeyBinds;
    StringFuncMap buttonFuncMap = {};
    StringFuncMap buttonReleaseMap = {};
    std::map<std::string, std::pair<std::function<void(float)>, float*> > sliderFuncMap = {};
    std::map<std::string, std::function<std::string()> > textVarMap = {};

    KeyFuncMap keyBinds = {};
    KeyFuncMap releasedKeyBinds = {};
    std::vector<sf::Keyboard::Key > pressedKeyStack = {};
    std::vector<sf::Keyboard::Key > releasedKeyStack = {};

    sf::RenderWindow &window;
    sf::View worldView = window.getDefaultView();
    sf::View GUIView = window.getDefaultView();

    UIContainer container{true};
    std::pair<bool,int> mouseOnUIWhenClicked{false, -1};
    bool mouseOnUI = false;
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

    virtual void update(sf::RenderWindow &_window);
    virtual void redraw(sf::RenderWindow &_window);
    virtual void initBindings();

    void loadUI(std::string filePath, UIContainer &_container);
    void loadKeybinds(std::string filePath,
                      std::string sceneType);

    virtual void load();
    virtual void unload();
    virtual void adjustViewSize(sf::Vector2u newSize);
    virtual void events(sf::Event &event);

    void keyEvents(sf::Event &event, std::vector<sf::Keyboard::Key > &_pressedKeyStack);
    void exePressedKeys(std::vector<sf::Keyboard::Key > &_pressedKeyStack,
                           KeyFuncMap &_keyBinds);
};

#endif // CLASS_SCENE_H
