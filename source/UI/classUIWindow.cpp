#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

#include "../../headers/classUIWindow.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"
#include "../../headers/classUIButton.h"

template<class T>
void UIWindow::addElement(std::string font, std::string str, int fontSize, sf::Vector2f position, T *var)
{
    UITextElement<T> *text = new UITextElement<T>{str, position, fixedToWindow, var};
    currentFont.loadFromFile(font);
    text->setFont(currentFont);
    text->setCharacterSize(fontSize);
    //text.setPosition(position);
    //text.setString(str);
    UITextElementBase *text2 = text;
    textArray.emplace_back(text2);
}


void UIWindow::addButton(std::string font, std::string text, int fontSize, sf::Vector2f position, sf::Vector2f bSize,
                                                void (*func)(), sf::Color color)
{
    //std::cout << fixedToWindow << "\n";
    UIButton *button = new UIButton{text, func, position, bSize, fixedToWindow, color};
    button->addElement<int>(font, text, fontSize, {10.0,10.0});
    buttonArray.emplace_back(button);
}



bool UIWindow::ifElementsCollide(sf::Rect<float> rectBound1, sf::Rect<float> rectBound2)
{
    if(rectBound1.intersects(rectBound2))
        return true;
    return false;
}


void UIWindow::renderElements(sf::RenderWindow &window, sf::View &GUIView)
{
    for(int i=0; i<textArray.size(); i++)
    {

        //if(textArray.at(i).getGlobalBounds().intersects(windowBox.getGlobalBounds()))
        {
            //sf::Rect<float> a = textArray.at(0)->getGlobalBounds();
            //sf::Rect<float> b = windowBox.getGlobalBounds();
            //std::cout << a.top << " " << a.left << " " << a.width << " " << a.height << "\n";
            //std::cout << b.top << " " << b.left << " " << b.width << " " << b.height << "\n";
            textArray.at(i)->updateElement(window, GUIView, origPosition);
            textArray.at(i)->textWrap(windowBox.getGlobalBounds());
            window.draw(*textArray.at(i));
        }
    }
    for(int i=0; i<buttonArray.size(); i++)
    {
        buttonArray.at(i)->updateElement(window, origPosition);
        buttonArray.at(i)->renderButton(window,GUIView);
    }
}


void UIWindow::renderWindow(sf::RenderWindow &window, sf::View &GUIView)
{

    if(fixedToWindow)
    {
        window.setView(GUIView);
        windowBox.setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(origPosition)));
    }
    else
    {
        windowBox.setPosition((origPosition));
    }
    window.draw(windowBox);
    renderElements(window, GUIView);
}


UIWindow::UIWindow(sf::Vector2f position, float width, float height, bool fixedToWin, bool draggable, sf::Color color) :
            origPosition{position}, width{width}, height{height}, color{color}, fixedToWindow{fixedToWin}, draggable{draggable}
{
    windowBox.setPosition(origPosition);
    windowBox.setSize(sf::Vector2f(width,height));
    windowBox.setFillColor(color);
}

void UIWindow::checkMouseIntersection(sf::RenderWindow &window)
{
    //std::cout << windowBox.getPosition() << "\n";
    if(fixedToWindow)
    {
        sf::Vector2f mousePosf = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
        if(origRect.contains(mousePosf))
        {
            //mouseOffset = {mousePosf.x - origRect.left, mousePosf.y - origRect.top};
            mouseIntersecting = true;
            for(int i=0; i<buttonArray.size(); i++)
            {
                buttonArray.at(i)->checkMouseIntersection(window);
                bool isIntersectingWithButton = buttonArray.at(i)->getIsMouseIntersecting();
                //std::cout << isIntersectingWithButton << "\n";
                if(isIntersectingWithButton)
                    mouseOnButtonWhenClicked = std::make_pair(isIntersectingWithButton, i);
                //std::cout << isIntersectingWithButton << "\n";

            }
            //std::cout << "intersecting" << "\n";
        }
        else
        {
            //mouseOffset = {0,0};
            mouseIntersecting = false;
        }
    }
    else
    {
        sf::Vector2f mousePosf = window.mapPixelToCoords(sf::Mouse::getPosition(window)); //static_cast<sf::Vector2f>(
       // mousePosf = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
        //std::cout << mousePosf << "\n";
        if(origRect.contains(mousePosf))
        {
            //std::cout << origRect.top << "\n";
            //mouseOffset = {mousePosf.x - origRect.left, mousePosf.y - origRect.top};
            mouseIntersecting = true;
            for(int i=0; i<buttonArray.size(); i++)
            {
                //std::cout << origRect.left << "\n";
                buttonArray.at(i)->checkMouseIntersection(window);
                bool isIntersectingWithButton = buttonArray.at(i)->getIsMouseIntersecting();
                //std::cout << isIntersectingWithButton << "\n";
                if(isIntersectingWithButton)
                    mouseOnButtonWhenClicked = std::make_pair(isIntersectingWithButton, i);

            }
            //std::cout << "intersecting" << "\n";
        }
        else
        {
            //mouseOffset = {0,0};
            mouseIntersecting = false;
        }
    }
    //std::cout << mouseOnButtonWhenClicked.first << "\n";
}

void UIWindow::resetButtonPair()
{
    mouseOnButtonWhenClicked = std::make_pair(false,-1);
}

bool UIWindow::getIsMouseIntersecting()
{
    return mouseIntersecting;
}

void UIWindow::clickIntersectedButton()
{
    int buttonIndex = std::get<1>(mouseOnButtonWhenClicked);
    if(buttonIndex != -1)
        buttonArray.at(buttonIndex)->clickButton();
}

std::pair<bool,int> UIWindow::getClickedButton()
{
    return mouseOnButtonWhenClicked;
}

void UIWindow::moveWindow(sf::RenderWindow &window, sf::Vector2i newPosition)
{
    if(draggable)
    {
        if(fixedToWindow)
            origPosition = static_cast<sf::Vector2f>(newPosition+mouseOffset);
        else
            origPosition = window.mapPixelToCoords(newPosition) + static_cast<sf::Vector2f>(mouseOffset);
        sf::Rect<float> newRect{origPosition,{width,height}};
        origRect = newRect;
    //std::cout << "Moving\n";
    //std::cout << mouseOffset << "\n";
    }
}

void UIWindow::changeOrigin(sf::RenderWindow &window, sf::Vector2i origin)
{
    //sf::Vector2i tempOffset =
    if(fixedToWindow)
        mouseOffset = static_cast<sf::Vector2i>(origPosition) - origin;
    else
        mouseOffset = static_cast<sf::Vector2i>(origPosition - window.mapPixelToCoords(origin));
    //std::cout << origPosition << "\n";
    //std::cout << window.mapPixelToCoords(origin) << "\n";
}

template void UIWindow::addElement<int>(std::string font, std::string str, int fontSize, sf::Vector2f position, int *var);
//template void UIWindow::addElement<const int>(std::string font, std::string str, int fontSize, sf::Vector2f position, const int *var);
template void UIWindow::addElement<float>(std::string font, std::string str, int fontSize, sf::Vector2f position, float *var);
template void UIWindow::addElement<bool>(std::string font, std::string str, int fontSize, sf::Vector2f position, bool *var);
template void UIWindow::addElement<sf::Vector2i>(std::string font, std::string str, int fontSize, sf::Vector2f position, sf::Vector2i *var);
template void UIWindow::addElement<sf::Vector2f>(std::string font, std::string str, int fontSize, sf::Vector2f position, sf::Vector2f *var);

