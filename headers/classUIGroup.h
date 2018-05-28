#ifndef CLASS_UIGROUP_H
#define CLASS_UIGROUP_H

#include "classUIWindow.h"

class UIGroup : public UIWindow
{
    sf::Vector2f currPosition = origPosition;

public:
    UIGroup(sf::Vector2f position, float width, float height, bool fixedToWin,
            sf::Color color = {150,0,0,100});

    void addButton(std::string font, std::string text, int fontSize,
                   sf::Vector2f position, sf::Vector2f bSize,
                   std::function<void()> const& func, sf::Color color = {80,80,80,150});

    void addSlider(sf::Vector2f position, float range, sf::Vector2f bSize,
                   sf::Vector2f physRange ={0,0}, std::function<void(float)> sliderFunc = nullptr,
                   float *variable = nullptr);

    void updateElement(sf::RenderWindow &window, sf::Vector2f parentPosition);

    void renderElements(sf::RenderWindow &window, sf::View &GUIView);
};

#endif
