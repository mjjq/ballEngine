#ifndef CLASS_SANDBOXSCENE_H
#define CLASS_SANDBOXSCENE_H

#include "classGameScene.h"

class SandboxScene : public GameScene
{
public:
    SandboxScene(sf::RenderWindow &_window,
                 sf::Time &_targetFTime,
                 sf::Time &_currentFTime,
                 float &_currentFPS);
    void update(sf::RenderWindow &_window);
    void load();
};

#endif // CLASS_SANDBOXSCENE_H
