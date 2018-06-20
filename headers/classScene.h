#ifndef CLASS_SCENE_H
#define CLASS_SCENE_H

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
public:
    void requestScene(SceneEnum scEnum);
    SceneEnum pollNextScene();
    virtual void update(sf::RenderWindow &window);
    virtual void redraw(sf::RenderWindow &window);
    virtual void load();
    virtual void unload();
    virtual void adjustViewSize(sf::Vector2u newSize);
    virtual void events(sf::Event &event);
};

#endif // CLASS_SCENE_H
