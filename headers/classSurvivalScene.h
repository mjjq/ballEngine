#ifndef CLASS_SURVIVALSCENE_H
#define CLASS_SURVIVALSCENE_H

#include "classGameScene.h"

class SurvivalScene : public GameScene
{
    bool startTheGame = false;
    sf::Time countDownTimer = sf::seconds(3.0f);

    void mouseWorldEvents(sf::Event &event);
public:
    SurvivalScene(sf::RenderWindow &_window,
                 sf::Time &_targetFTime,
                 sf::Time &_currentFTime,
                 float &_currentFPS);
    void update(sf::RenderWindow &_window);
    void load();
};

#endif
