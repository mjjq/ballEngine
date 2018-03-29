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
            UIWindow(position, bSize.x, bSize.y, fixedToWin, color)
{
    //addElement(font)
}

void UIButton::clickButton()
{

}

void UIButton::renderButton(sf::RenderWindow &window, sf::View &GUIView)
{
    window.draw(windowBox);
    textArray.at(0)->updateElement(window, GUIView, currPosition);
    window.draw(*textArray.at(0));
   // std::cout << static_cast<std::string>(textArray.at(0)->getString()) << "\n";
    /*for(int i=0; i<textArray.size(); i++)
    {

        //if(textArray.at(i).getGlobalBounds().intersects(windowBox.getGlobalBounds()))
        {
            //sf::Rect<float> a = textArray.at(0)->getGlobalBounds();
            //sf::Rect<float> b = windowBox.getGlobalBounds();
            //std::cout << a.top << " " << a.left << " " << a.width << " " << a.height << "\n";
            //std::cout << b.top << " " << b.left << " " << b.width << " " << b.height << "\n";
            //textArray.at(i)->updateElement(window, GUIView, origPosition);
            textArray.at(i)->textWrap(windowBox.getGlobalBounds());
            window.draw(*textArray.at(i));
        }
    }*/
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
