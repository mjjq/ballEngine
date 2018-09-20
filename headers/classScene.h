#ifndef CLASS_SCENE_H
#define CLASS_SCENE_H

#include "classUIContainer.h"
#include "classKeyBinds.h"
#include <fstream>

enum class SceneEnum
{
    SCENE_MENU,
    SCENE_GAME,
    SCENE_PAUSEMENU,
    LAST
};

class Scene
{
    SceneEnum nextScene = SceneEnum::LAST;

protected:
    std::map<std::string, std::function<void()> > buttonFuncMap = {};
    std::map<std::string, std::pair<std::function<void(float)>, float*> > sliderFuncMap = {};
    std::map<std::string, std::function<std::string()> > textVarMap = {};

    std::vector<sf::Keyboard::Key > pressedKeyStack = {};

public:
    void requestScene(SceneEnum scEnum);
    SceneEnum pollNextScene();
    virtual void update(sf::RenderWindow &window);
    virtual void redraw(sf::RenderWindow &window);
    virtual void initBindings();
    void loadUI(std::string filePath, UIContainer &container);
    void loadKeybinds(std::string filePath, StringFuncMap &sfMap, KeyFuncMap &keyMap);
    virtual void load();
    virtual void unload();
    virtual void adjustViewSize(sf::Vector2u newSize);
    virtual void events(sf::Event &event);
};

#endif // CLASS_SCENE_H
