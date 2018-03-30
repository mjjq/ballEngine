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
    sf::Color clickedColor = {30,30,30,150};
    sf::Color unclickedColor;

    bool displayElement = true;
    //bool fixedToWindow;

public:
    UIButton(std::string text, std::function<void()> clickFunc, sf::Vector2f position, sf::Vector2f bSize, bool fixedToWin, sf::Color color = {80,80,80,150});
    void clickButton();
    void unclickButton();
    void renderButton(sf::RenderWindow &window, sf::View &GUIView);
    void updateElement(sf::RenderWindow &window, sf::Vector2f parentPosition);
    //void updateElement(sf::RenderWindow &window, sf::View &GUIView, sf::Vector2f parentPosition);

};



#endif
