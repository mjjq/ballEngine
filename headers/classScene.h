#ifndef CLASS_SCENE_H
#define CLASS_SCENE_H

class Scene
{
public:
    virtual void update(sf::RenderWindow &window);
    virtual void redraw(sf::RenderWindow &window);
    virtual void load();
    virtual void unload();
    virtual void adjustViewSize(sf::Vector2u newSize);
    virtual void events(sf::Event &event);
};

#endif // CLASS_SCENE_H
