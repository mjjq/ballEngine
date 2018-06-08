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
#include "../../headers/classUIGroup.h"

void UIWindow::addGroup(sf::Vector2f position, float width,
                        float height, bool fixedToWin, bool draggable)
{
    std::unique_ptr<UIGroup> newGroup = std::make_unique<UIGroup>(position, width, height, fixedToWin, color);
    groupArray.emplace_back(std::move(newGroup));
}

template<class T>
void UIWindow::addElement(std::string font, std::string str,
                          int fontSize, sf::Vector2f position, T *var)
{
    std::unique_ptr<UITextElement<T>> text = std::make_unique<UITextElement<T>>(str, position, fixedToWindow,
                                    var, !isButton, windowBox.getLocalBounds());
    currentFont.loadFromFile(font);
    text->setFont(currentFont);
    text->setCharacterSize(fontSize);
    //UITextElementBase *text2 = text;
    textArray.emplace_back(std::move(text));
}


void UIWindow::addButton(std::string font, std::string text, int fontSize,
                         sf::Vector2f position, sf::Vector2f bSize,
                         std::function<void()> const& func, sf::Color color,
                         bool changeState)
{
    addGroup(position, bSize.x, bSize.y, fixedToWindow, false);
    groupArray.back()->addButton(font, text, fontSize, {0,0}, bSize, func,
                                                        color, changeState);
}

void UIWindow::addSlider(sf::Vector2f position, float range, sf::Vector2f bSize,
                         sf::Vector2f physRange, std::function<void(float)> sliderFunc,
                         float *variable)
{
    addGroup(position, range+bSize.x, bSize.y, fixedToWindow, false);
    float thickness = 2.0f;

    groupArray.back()->addSlider({0,0}, range, thickness, bSize,
                                 physRange, sliderFunc, variable);
}

bool UIWindow::ifElementsCollide(sf::Rect<float> rectBound1, sf::Rect<float> rectBound2)
{
    if(rectBound1.intersects(rectBound2))
        return true;
    return false;
}


void UIWindow::renderElements(sf::RenderWindow &window, sf::View &GUIView)
{
    for(unsigned int i=0; i<textArray.size(); ++i)
    {
        textArray.at(i)->updateElement(window, GUIView, origPosition);
        //textArray.at(i)->textWrap(windowBox.getGlobalBounds());
        window.draw(*textArray.at(i));
        //std::cout << textArray.at(i)->getPosition()<< "\n";
    }
    for(unsigned int i=0; i<buttonArray.size(); ++i)
    {
        buttonArray.at(i)->updateElement(window, origPosition);
        buttonArray.at(i)->renderButton(window,GUIView);
    }
    for(unsigned int i=0; i<groupArray.size(); ++i)
    {
        groupArray.at(i)->updateElement(window, origPosition);
        groupArray.at(i)->renderElements(window, GUIView);
    }
}


void UIWindow::renderWindow(sf::RenderWindow &window, sf::View &GUIView)
{

    if(fixedToWindow)
    {
        window.setView(GUIView);
        windowBox.setPosition(window.mapPixelToCoords
            (static_cast<sf::Vector2i>(origPosition)));
    }
    else
    {
        windowBox.setPosition((origPosition));
    }
    window.draw(windowBox);
    renderElements(window, GUIView);
}


UIWindow::UIWindow(sf::Vector2f position, float width, float height, bool fixedToWin,
                bool draggable, sf::Color color) : origPosition{position}, width{width},
                height{height}, color{color}, fixedToWindow{fixedToWin},
                draggable{draggable}
{
    windowBox.setPosition(origPosition);
    windowBox.setSize(sf::Vector2f(width,height));
    windowBox.setFillColor(color);
}

UIWindow::~UIWindow() {}

void UIWindow::checkMouseIntersection(sf::RenderWindow &window)
{
    mouseIntersecting = false;
    resetButtonPair();
    sf::Vector2f mousePosf;

    if(fixedToWindow)
        mousePosf = static_cast<sf::Vector2f>
                        (sf::Mouse::getPosition(window));
    else
        mousePosf = window.mapPixelToCoords
                        (sf::Mouse::getPosition(window));

    if(origRect.contains(mousePosf))
        mouseIntersecting = true;
    else
        mouseIntersecting = false;

    for(unsigned int i=0; i<buttonArray.size(); ++i)
    {
        buttonArray.at(i)->checkMouseIntersection(window);
        bool isIntersectingWithButton = buttonArray.at(i)->getIsMouseIntersecting();
        if(isIntersectingWithButton)
            mouseOnButton = std::make_pair(isIntersectingWithButton, i);
    }

    for(unsigned int j=0; j<groupArray.size(); ++j)
    {
        groupArray.at(j)->checkMouseIntersection(window);
        bool isIntWithGroup = groupArray.at(j)->getIsMouseIntersecting();
        if(isIntWithGroup)
            mouseOnGroup = std::make_pair(isIntWithGroup, j);
    }
}

void UIWindow::resetButtonPair()
{
    //mouseOnButtonWhenClicked = std::make_pair(false,-1);
    mouseOnButton = std::make_pair(false,-1);
    mouseOnGroup = std::make_pair(false,-1);
    //std::cout << mouseOnButtonWhenClicked.second << "\n";
}

void UIWindow::releaseClickedButton()
{
    bool buttonBool = mouseOnButtonWhenClicked.first;
    int buttonIndex = mouseOnButtonWhenClicked.second;
    if(buttonBool)
    {
        buttonArray.at(buttonIndex)->releaseClickedButton();
    }

    bool groupBool = mouseOnGroupWhenClicked.first;
    int groupIndex = mouseOnGroupWhenClicked.second;
    if(groupBool)
    {
        groupArray.at(groupIndex)->releaseClickedButton();
    }
    mouseIntersecting = false;
    mouseOnButtonWhenClicked = std::make_pair(false,-1);
    mouseOnGroupWhenClicked = std::make_pair(false, -1);
    //std::cout << "Button on Release: " << buttonIndex << "\n";
}

bool UIWindow::getIsMouseIntersecting()
{
    return mouseIntersecting;
}

void UIWindow::clickIntersectedButton(sf::RenderWindow &window)
{
    int buttonIndex = std::get<1>(mouseOnButton);
    int groupIndex = std::get<1>(mouseOnGroup);
    std::cout << "Button on Click: " << groupIndex << "\n";
    if(buttonIndex != -1)
    {
        mouseOnButtonWhenClicked = mouseOnButton;
        buttonArray.at(buttonIndex)->clickIntersectedButton(window);
    }
    else if(groupIndex != -1)
    {
        mouseOnGroupWhenClicked = mouseOnGroup;
        groupArray.at(groupIndex)->clickIntersectedButton(window);
    }
}

std::pair<bool,int> UIWindow::getClickedButton()
{
    return mouseOnGroupWhenClicked;
}

void UIWindow::moveWindow(sf::RenderWindow &window, sf::Vector2i newPosition)
{
    if(draggable)
    {
        if(fixedToWindow)
            origPosition = static_cast<sf::Vector2f>(newPosition+mouseOffset);
        else
            origPosition = window.mapPixelToCoords(newPosition) +
                            static_cast<sf::Vector2f>(mouseOffset);
        sf::Rect<float> newRect{origPosition,{width,height}};
        origRect = newRect;
    }
}

void UIWindow::changeOrigin(sf::RenderWindow &window, sf::Vector2i origin)
{
    if(fixedToWindow)
        mouseOffset = static_cast<sf::Vector2i>(origPosition) - origin;
    else
        mouseOffset = static_cast<sf::Vector2i>(origPosition -
                                window.mapPixelToCoords(origin));
}

void UIWindow::destroyAllElements()
{
    textArray.clear();
    buttonArray.clear();
    groupArray.clear();
}

template void UIWindow::addElement<int>(std::string font, std::string str,
                                int fontSize, sf::Vector2f position, int *var);
template void UIWindow::addElement<float>(std::string font, std::string str,
                                int fontSize, sf::Vector2f position, float *var);
template void UIWindow::addElement<bool>(std::string font, std::string str,
                                int fontSize, sf::Vector2f position, bool *var);
template void UIWindow::addElement<sf::Vector2i>(std::string font, std::string str,
                                int fontSize, sf::Vector2f position, sf::Vector2i *var);
template void UIWindow::addElement<sf::Vector2f>(std::string font, std::string str,
                                int fontSize, sf::Vector2f position, sf::Vector2f *var);
template void UIWindow::addElement<Integrators>(std::string font, std::string str,
                                int fontSize, sf::Vector2f position, Integrators *var);

