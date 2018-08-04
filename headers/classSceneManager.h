#ifndef CLASS_SCENEMAN_H
#define CLASS_SCENEMAN_H

#include "classScene.h"
#include "classGameScene.h"
#include "classMenuScene.h"
#include "classPauseMenuScene.h"
#include "classUIContainer.h"
#include "classUIWindow.h"

class SceneManager
{
    std::deque<float> previousFrames;
    sf::Time currentFrameTime = sf::seconds(1.0f/60.0f);
    sf::Time targetFTime = sf::seconds(1.0f/60.0f);
    sf::Time timeSinceFSample = sf::milliseconds(0);
    float currentFPS = 0.0f;

    sf::Clock frameClock;

    sf::Vector2u windowSize{800,800};
    sf::Vector2u prevWinSize{800,800};
    sf::Vector2i prevWinPos;
    bool isFullScreen = false;

    sf::RenderWindow window{sf::VideoMode(windowSize.x, windowSize.y), "N-body"};
    sf::View worldView = window.getDefaultView();
    sf::View GUIView = window.getDefaultView();
    sf::ContextSettings settings;

    GameScene game{window, targetFTime, currentFrameTime, currentFPS};
    MenuScene menu{window, currentFrameTime, currentFPS};
    PauseMenuScene pauseMenu{window, currentFrameTime, currentFPS};
    Scene *currScene = &menu;

    SceneEnum thisSceneEnum = SceneEnum::LAST;
    SceneEnum nextSceneEnum = SceneEnum::LAST;

    void limitFramerate(int framerate);
    void updateFPS(sf::Time interval, float framerate);
    sf::Time sampleNextFrame(sf::Time frameTime);

    void toggleFullScreen(Scene &currScene);
    void setAALevel(unsigned int level, Scene &currScene);

    void loadNextScene(SceneEnum nextScene);

public:
    SceneManager();
    void mainLoop();
};

#endif // CLASS_SCENEMAN_H
