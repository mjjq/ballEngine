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
    //sliderLine.setOrigin({-width/2.0f,-(height-thickness)/2.0f});
    //sliderLine.setFillColor(sliderColor);
}

void UISlider::renderButton(sf::RenderWindow &window, sf::View &GUIView)
{
    //window.draw(sliderLine);
    window.draw(windowBox);
}

void UISlider::clickIntersectedButton(sf::RenderWindow &window)
{
    buttonDown = true;
    if(fixedToWindow)
        changeOrigin(sf::Mouse::getPosition(window));
    else
    {
        sf::Vector2i tempOrigin = static_cast<sf::Vector2i>
            (window.mapPixelToCoords(sf::Mouse::getPosition(window)));
        changeOrigin(tempOrigin);
    }
}

void UISlider::clickIntersectedButton()
{
    buttonDown = true;
    changeOrigin(static_cast<sf::Vector2i>(sf::Vector2f
                {currButtonPosition.x + width/2, currButtonPosition.y + height/2}));
}

void UISlider::releaseClickedButton()
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
        //sliderLine.setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(currPosition)));
        windowBox.setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(currButtonPosition)));
    }
    else
    {
        //sliderLine.setPosition(currPosition);
        windowBox.setPosition(currButtonPosition);
    }

    if(buttonDown)
        windowBox.setFillColor(clickedColor);
    else if(mouseIntersecting && !buttonDown)
        windowBox.setFillColor(mouseOverColor);
    else
        windowBox.setFillColor(unclickedColor);
}

void UISlider::changeOrigin(sf::Vector2i origin)
{
    //if(fixedToWindow)
        mouseOffset = static_cast<sf::Vector2i>(currButtonPosition) - origin;
    /*else
        mouseOffset = static_cast<sf::Vector2i>(currButtonPosition - window.mapPixelToCoords(origin));*/
}

void UISlider::checkMouseIntersection(sf::RenderWindow &window)
{
    mouseIntersecting = false;
    resetButtonPair();
    sf::Vector2f mousePosf;

    if(fixedToWindow)
        mousePosf = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    else
        mousePosf = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if(origRect.contains(mousePosf))
    {
        mouseIntersecting = true;
        for(unsigned int i=0; i<buttonArray.size(); i++)
        {
            buttonArray.at(i)->checkMouseIntersection(window);
            bool isIntersectingWithButton = buttonArray.at(i)->getIsMouseIntersecting();
            if(isIntersectingWithButton)
                mouseOnButton = std::make_pair(isIntersectingWithButton, i);
        }
    }
    else
        mouseIntersecting = false;
}
