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
#include "../../headers/classUISlider.h"

template<class T>
void UIWindow::addElement(std::string font, std::string str, int fontSize, sf::Vector2f position, T *var)
{
    //UITextElement<T> *text;
    UITextElement<T> *text = new UITextElement<T>{str, position, fixedToWindow, var, !isButton, windowBox.getLocalBounds()};
    currentFont.loadFromFile(font);
    text->setFont(currentFont);
    text->setCharacterSize(fontSize);
    //text.setPosition(position);
    //text.setString(str);
    UITextElementBase *text2 = text;
    textArray.emplace_back(text2);
}


void UIWindow::addButton(std::string font, std::string text, int fontSize, sf::Vector2f position, sf::Vector2f bSize,
                                                std::function<void()> const& func, sf::Color color)
{
    //std::cout << fixedToWindow << "\n";
    UIButton *button = new UIButton{font, text, fontSize, func, position, bSize, fixedToWindow, color};
    //button->addElement<int>(font, text, fontSize, {10.0,10.0});
    buttonArray.emplace_back(button);
}

void UIWindow::addSlider(sf::Vector2f position, float range, sf::Vector2f bSize,
                         sf::Vector2f physRange, std::function<void(float)> sliderFunc)
{
    UISlider *slider = new UISlider{position, bSize,
                   fixedToWindow, range, {70,70,70,255}, physRange, sliderFunc};
    buttonArray.emplace_back(slider);
}

bool UIWindow::ifElementsCollide(sf::Rect<float> rectBound1, sf::Rect<float> rectBound2)
{
    if(rectBound1.intersects(rectBound2))
        return true;
    return false;
}


void UIWindow::renderElements(sf::RenderWindow &window, sf::View &GUIView)
{
    for(UITextElementBase *textElement : textArray)
    {

        //if(textArray.at(i).getGlobalBounds().intersects(windowBox.getGlobalBounds()))
        {
            //sf::Rect<float> a = textArray.at(0)->getGlobalBounds();
            //sf::Rect<float> b = windowBox.getGlobalBounds();
            //std::cout << a.top << " " << a.left << " " << a.width << " " << a.height << "\n";
            //std::cout << b.top << " " << b.left << " " << b.width << " " << b.height << "\n";
            textElement->updateElement(window, GUIView, origPosition);
            textElement->textWrap(windowBox.getGlobalBounds());
            window.draw(*textElement);
        }
    }
    for(UIButton *button : buttonArray)
    {
        button->updateElement(window, origPosition);
        button->renderButton(window,GUIView);
    }
    /*for(int i=0; i<sliderArray.size(); ++i)
    {
        sliderArray.at(i)->updateElement(window, origPosition);
        //sliderArray.at(i)->renderSlider(window, GUIView);
    }*/
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
    mouseIntersecting = false;
    resetButtonPair();

    if(fixedToWindow)
    {
        sf::Vector2f mousePosf = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
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
    else
    {
        sf::Vector2f mousePosf = window.mapPixelToCoords(sf::Mouse::getPosition(window));
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
}

void UIWindow::resetButtonPair()
{
    //mouseOnButtonWhenClicked = std::make_pair(false,-1);
    mouseOnButton = std::make_pair(false,-1);
    //std::cout << mouseOnButtonWhenClicked.second << "\n";
}

void UIWindow::releaseClickedButton()
{
    bool buttonBool = mouseOnButtonWhenClicked.first;
    int buttonIndex = mouseOnButtonWhenClicked.second;
    if(buttonBool)
    {
        buttonArray.at(buttonIndex)->releaseButton();
    }
    mouseIntersecting = false;
    mouseOnButtonWhenClicked = std::make_pair(false,-1);
    //std::cout << "Button on Release: " << buttonIndex << "\n";
}

bool UIWindow::getIsMouseIntersecting()
{
    return mouseIntersecting;
}

void UIWindow::clickIntersectedButton(sf::RenderWindow &window)
{
    int buttonIndex = std::get<1>(mouseOnButton);
    std::cout << "Button on Click: " << buttonIndex << "\n";
    if(buttonIndex != -1)
    {
        mouseOnButtonWhenClicked = mouseOnButton;
        buttonArray.at(buttonIndex)->clickButton(window);
    }
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
template void UIWindow::addElement<Integrators>(std::string font, std::string str, int fontSize, sf::Vector2f position, Integrators *var);

