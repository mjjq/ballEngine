#ifndef CLASS_BUTTON_H
#define CLASS_BUTTON_H

#include <functional>

#include "classUIWindow.h"
#include "structs.h"

class UIButton : public UIWindow
{
protected:
    sf::Vector2f currPosition = origPosition;
    sf::Color unclickedColor;
    sf::Color clickedColor = unclickedColor - sf::Color{20,20,20,0};
    sf::Color mouseOverColor = unclickedColor + sf::Color{20,20,20,0};
    std::function<void()> upFunc = [&]{};
    std::function<void()> downFunc = [&]{};

    bool buttonDown = false;

    bool changeState = true;

    bool displayElement = true;

    bool upExOverride = false;
    //bool fixedToWindow;

public:
    UIButton(ButtonParams bParams, WindowParams &wParams, bool changeState = true);

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
