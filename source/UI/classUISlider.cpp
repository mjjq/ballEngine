#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

#include "../../headers/classUISlider.h"
#include "../../headers/stringConversion.h"


/*UISlider::UISlider(sf::Vector2f position, int range, sf::Vector2f bSize, bool fixedToWin) :
    currPosition{position}, range{range}, buttonSize{bSize}, fixedToWin{fixedToWin}
{

}*/

UISlider::UISlider(sf::Vector2f position, sf::Vector2f bSize,
                   bool fixedToWin, float range, sf::Color color,
                   sf::Vector2f physRange, std::function<void(float)> slideFunc) :
                   UIButton("", "", 1, [&]{}, position, bSize, fixedToWin, color),
                   range{range}, sliderFunc{slideFunc}, physRange{physRange}
{
    sliderLine.setOrigin({-width/2.0f,-(height-thickness)/2.0f});
    sliderLine.setFillColor(sliderColor);
}

void UISlider::renderButton(sf::RenderWindow &window, sf::View &GUIView)
{
    window.draw(sliderLine);
    window.draw(windowBox);
}

void UISlider::clickButton(sf::RenderWindow &window)
{
    buttonDown = true;
    changeOrigin(window, sf::Mouse::getPosition(window));
}

void UISlider::releaseButton()
{
    buttonDown = false;
    //currPosition = buttonPosition;
    //std::cout << currPosition << "\n";
}

void UISlider::updateElement(sf::RenderWindow &window, sf::Vector2f parentPosition)
{
    //std::cout << fixedToWindow << "\n";
    currPosition = origPosition + parentPosition;
    currButtonPosition = currPosition + buttonPosOnRel;
    if(buttonDown)
    {
        float newPosX{0.0f};
        if(fixedToWindow)
            newPosX = sf::Mouse::getPosition(window).x + mouseOffset.x - currPosition.x;
        else
            newPosX = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x + mouseOffset.x - currPosition.x;


        if(newPosX > 0.0f && newPosX < range)
        {
            if(sliderFunc != nullptr)
                sliderFunc((physRange.y-physRange.x)*newPosX/range+physRange.x);
            buttonPosOnRel = sf::Vector2f{newPosX, 0};
        }
        else if(newPosX <= 0.0f)
        {
            if(sliderFunc != nullptr)
                sliderFunc(physRange.x);
            buttonPosOnRel = sf::Vector2f{0,0};
        }
        else
        {
            if(sliderFunc != nullptr)
                sliderFunc(physRange.y);
            buttonPosOnRel = sf::Vector2f{range, 0};
        }
        //std::cout << buttonDown << "yes\n";
    }

    //std::cout << buttonPosition << "\n";
    sf::Rect<float> newButtonRect{currButtonPosition,{width,height}};
    origRect = newButtonRect;
    if(fixedToWindow)
    {
        sliderLine.setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(currPosition)));
        windowBox.setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(currButtonPosition)));
    }
    else
    {
        sliderLine.setPosition(currPosition);
        windowBox.setPosition(currButtonPosition);
    }

    if(buttonDown)
        windowBox.setFillColor(clickedColor);
    else if(mouseIntersecting && !buttonDown)
        windowBox.setFillColor(mouseOverColor);
    else
        windowBox.setFillColor(unclickedColor);

    //std::cout << currPosition << "\n";
}

void UISlider::changeOrigin(sf::RenderWindow &window, sf::Vector2i origin)
{
    //sf::Vector2i tempOffset =
    if(fixedToWindow)
        mouseOffset = static_cast<sf::Vector2i>(currButtonPosition) - origin;
    else
        mouseOffset = static_cast<sf::Vector2i>(currButtonPosition - window.mapPixelToCoords(origin));
    //std::cout << origPosition << "\n";
    //std::cout << window.mapPixelToCoords(origin) << "\n";
}
