#ifndef CLASS_BUTTON_H
#define CLASS_BUTTON_H

#include <functional>

#include "classUIWindow.h"

class UIButton : public UIWindow
{
protected:
    //sf::Font displayFont;
    //sf::Vector2f origPosition;
    //sf::RectangleShape buttonRect;
    //std::string buttonText;
    sf::Vector2f currPosition = origPosition;
    sf::Color clickedColor = {60,60,60,150};
    sf::Color unclickedColor;
    sf::Color mouseOverColor = {100,100,100,150};
    std::function<void()> clickFunc;

    bool buttonDown = false;

    bool displayElement = true;
    //bool fixedToWindow;

public:
    UIButton(std::string font, std::string text, int fontSize, std::function<void()> const& clickFunc, sf::Vector2f position, sf::Vector2f bSize, bool fixedToWin, sf::Color color = {80,80,80,150});

    virtual void clickButton(sf::RenderWindow &window);
    virtual void releaseButton();
    virtual void renderButton(sf::RenderWindow &window, sf::View &GUIView);
    virtual void updateElement(sf::RenderWindow &window, sf::Vector2f parentPosition);
    //void updateElement(sf::RenderWindow &window, sf::View &GUIView, sf::Vector2f parentPosition);

};



#endif
