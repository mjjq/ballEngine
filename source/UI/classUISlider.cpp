#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

#include "../../headers/classUISlider.h"
#include "../../headers/stringConversion.h"


UISlider::UISlider(sf::Vector2f position, sf::Vector2f bSize,
                   bool fixedToWin, float range, sf::Color color,
                   sf::Vector2f physRange, std::function<void(float)> slideFunc,
                   float *variable) :
                   UIButton("", "", 1, [&]{}, position, bSize, fixedToWin, color),
                   range{range}, sliderFunc{slideFunc}, physRange{physRange}, variable{variable}
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
}

void UISlider::updateElement(sf::RenderWindow &window, sf::Vector2f parentPosition)
{
    currPosition = origPosition + parentPosition;
    currButtonPosition = currPosition + buttonPosOnRel;
    if(!buttonDown && variable != nullptr)
    {
        float newPosX = range*(*variable - physRange.x)/(physRange.y - physRange.x);
        if(newPosX < 0.0f)
            buttonPosOnRel = sf::Vector2f{0, 0};
        else if(newPosX > range)
            buttonPosOnRel = sf::Vector2f{range, 0};
        else
            buttonPosOnRel = sf::Vector2f{newPosX, 0};
    }
    else if(buttonDown)
    {
        float newPosX{0.0f};
        if(fixedToWindow)
            newPosX = sf::Mouse::getPosition(window).x + mouseOffset.x - currPosition.x;
        else
            newPosX = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x + mouseOffset.x - currPosition.x;


        if(newPosX > 0.0f && newPosX < range)
        {
            if(sliderFunc != nullptr)
                sliderFunc((physRange.y-physRange.x)*newPosX/range + physRange.x);
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
    }

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
}

void UISlider::changeOrigin(sf::RenderWindow &window, sf::Vector2i origin)
{
    if(fixedToWindow)
        mouseOffset = static_cast<sf::Vector2i>(currButtonPosition) - origin;
    else
        mouseOffset = static_cast<sf::Vector2i>(currButtonPosition - window.mapPixelToCoords(origin));
}