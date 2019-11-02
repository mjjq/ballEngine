#include <deque>
#include <SFML/Graphics.hpp>
#include "classScene.h"
#include <vector>

class WindowManager
{
    std::vector<float> previousFrames;
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

public:
    void limitFramerate(int framerate);
    void updateFPS(sf::Time interval);
    void updateFPS(sf::Time interval, float framerate);
    sf::Time sampleNextFrame(sf::Time frameTime);

    void toggleFullScreen(Scene &currScene);
    void setAALevel(unsigned int level, Scene &currScene);

    sf::RenderWindow& getWindow();
    sf::Time& getCurrFrameTime();
    sf::Time& getTargFrameTime();
    float& getCurrentFPS();
    sf::Clock& getFrameClock();
    sf::Vector2u getWindowSize();
};
