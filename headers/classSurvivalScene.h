#ifndef CLASS_SURVIVALSCENE_H
#define CLASS_SURVIVALSCENE_H

#include "classGameScene.h"

class SurvivalScene : public GameScene
{
    struct GameLogic
    {
        bool startTheGame = false;
        bool gameOver = false;
        bool allowPlayerMovement = false;

        sf::Time countDownTimer = sf::seconds(3.0f);
        sf::Time upTimer = sf::seconds(0.0f);
    };
    GameLogic gameLogic;

    void mouseWorldEvents(sf::Event &event);

    void spawnRandomBalls(int nOBalls,
                          sf::Vector2f position,
                          float boundingRadius,
                          float ballRadius,
                          float ballMass,
                          float maxSpeed);

    void startGame();
    void endGame();
    void restartGame();

public:
    SurvivalScene(sf::RenderWindow &_window,
                 sf::Time &_targetFTime,
                 sf::Time &_currentFTime,
                 float &_currentFPS);
    void update(sf::RenderWindow &_window);
    void load();
};

#endif
