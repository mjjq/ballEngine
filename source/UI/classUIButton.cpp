#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

#include "../../headers/classUIButton.h"
#include "../../headers/classUIWindow.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"

UIButton::UIButton(std::string font, std::string text, int fontSize,
                   std::function<void()> const& upFunc, sf::Vector2f position,
                   sf::Vector2f bSize, bool fixedToWin, sf::Color color,
                   bool changeState) :
            UIWindow(position, bSize.x, bSize.y, fixedToWin, false, color),
            unclickedColor{color}, upFunc{upFunc}, changeState{changeState}
{

    isButton = true;
    if(font != "")
        addElement<int>(font, text, fontSize, {0.0,0.0});
    else
        addElement<int>("./fonts/cour.ttf", "", 1, {0.0,0.0});
    sf::Rect<float> buttonBounds = windowBox.getLocalBounds();
    sf::Rect<float> textBounds = textArray.at(0)->getLocalBounds();

    sf::Vector2i newOrigin = sf::Vector2i(textBounds.width/2 + textBounds.left,
                                          textBounds.height/2 + textBounds.top);

    textArray.at(0)->setOrigin(newOrigin.x, newOrigin.y);
    textArray.at(0)->setOrigPosition({buttonBounds.width/2,buttonBounds.height/2});


}

void UIButton::clickIntersectedButton(sf::RenderWindow &window)
{
    downFunc();
    buttonDown = true;
    //clickFunc();
}

void UIButton::clickIntersectedButton()
{
    //std::cout << &downFunc << "\n";
    downFunc();
    buttonDown = true;
}

void UIButton::releaseClickedButton()
{
    if( (mouseIntersecting && buttonDown) || upExOverride)
    {
        //std::cout << "Button is " << static_cast<std::string>(textArray.at(0)->getString()) << "\n";
        upFunc();
    }
    buttonDown = false;
}

void UIButton::renderButton(sf::RenderWindow &window, sf::View &GUIView)
{
    window.draw(windowBox);
    textArray.at(0)->updateElement(window, GUIView, currPosition);
    window.draw(*textArray.at(0));
}

void UIButton::updateElement(sf::RenderWindow &window, sf::Vector2f parentPosition)
{
    //std::cout << mouseIntersecting << "\n";
    currPosition = origPosition+parentPosition;
    //std::cout << parentPosition << "\n";
    sf::Rect<float> newRect{currPosition,{width,height}};
    origRect = newRect;
    if(fixedToWindow)
        windowBox.setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(currPosition)));
    else
        windowBox.setPosition(currPosition);

    if(mouseIntersecting && buttonDown && changeState)
        windowBox.setFillColor(clickedColor);
    else if(mouseIntersecting && !buttonDown && changeState)
        windowBox.setFillColor(mouseOverColor);
    else
        windowBox.setFillColor(unclickedColor);

    //std::cout << currPosition << "\n";

}

void UIButton::setDownFunction(std::function<void()> const& func)
{
    downFunc = func;
}

void UIButton::setUpFunction(std::function<void()> const& func)
{
    upFunc = func;
}

void UIButton::setUpExOverride(bool value)
{
    upExOverride = value;
}

