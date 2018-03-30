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

UIButton::UIButton(std::string text, std::function<void()> clickFunc, sf::Vector2f position, sf::Vector2f bSize, bool fixedToWin, sf::Color color) :
            UIWindow(position, bSize.x, bSize.y, fixedToWin, false, color), unclickedColor{color}
{
    //addElement(font)
}

void UIButton::clickButton()
{
    std::cout << "Button is " << static_cast<std::string>(textArray.at(0)->getString()) << "\n";
    //windowBox.setFillColor(clickedColor);
}

void UIButton::unclickButton()
{
    windowBox.setFillColor(unclickedColor);
}

void UIButton::renderButton(sf::RenderWindow &window, sf::View &GUIView)
{
    window.draw(windowBox);
    textArray.at(0)->updateElement(window, GUIView, currPosition);
    window.draw(*textArray.at(0));
}

void UIButton::updateElement(sf::RenderWindow &window, sf::Vector2f parentPosition)
{
    //std::cout << fixedToWindow << "\n";
    currPosition = origPosition+parentPosition;
    //std::cout << parentPosition << "\n";
    sf::Rect<float> newRect{currPosition,{width,height}};
    origRect = newRect;
    if(fixedToWindow)
        windowBox.setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(currPosition)));
    else
        windowBox.setPosition(currPosition);
    //std::cout << currPosition << "\n";

}
