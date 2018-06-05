#ifndef CLASS_BUTTON_H
#define CLASS_BUTTON_H

#include <functional>

#include "classUIWindow.h"

class UIButton : public UIWindow
{
protected:
    sf::Vector2f currPosition = origPosition;
    sf::Color clickedColor = {60,60,60,150};
    sf::Color unclickedColor;
    sf::Color mouseOverColor = {100,100,100,150};
    std::function<void()> upFunc;
    std::function<void()> downFunc = [&]{};

    bool buttonDown = false;

    bool changeState = true;

    bool displayElement = true;

    bool upExOverride = false;
    //bool fixedToWindow;

public:
    UIButton(std::string font, std::string text, int fontSize,
             std::function<void()> const& upFunc, sf::Vector2f position,
             sf::Vector2f bSize, bool fixedToWin, sf::Color color = {80,80,80,150},
             bool changeState = true);

    virtual void clickIntersectedButton(sf::RenderWindow &window);
    virtual void clickIntersectedButton();
    virtual void releaseClickedButton();
    virtual void renderButton(sf::RenderWindow &window, sf::View &GUIView);
    virtual void updateElement(sf::RenderWindow &window, sf::Vector2f parentPosition);
    void setDownFunction(std::function<void()> const& func);
    void setUpFunction(std::function<void()> const& func);
    void setUpExOverride(bool value);

};



#endif
