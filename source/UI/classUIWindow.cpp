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
#include "../../headers/structs.h"

//sf::Vector2u UIWindow::appWinSize;

void UIWindow::addGroup(WindowParams &wParams)
{
    std::unique_ptr<UIGroup> newGroup = std::make_unique<UIGroup>(wParams);
    groupArray.push_back(std::move(newGroup));
}

template<class T>
void UIWindow::addElement(TextElParams<T> &tParams)
{
    //dynamic_cast<TextElParams<T>>(tParams);
    std::unique_ptr<UITextElement<T>> text = std::make_unique<UITextElement<T>>
            (tParams.str, tParams.position, fixedToWindow, tParams.var, !isButton,
             windowBox.getLocalBounds());
    currentFont.loadFromFile(tParams.font);
    text->setFont(currentFont);
    text->setCharacterSize(tParams.fontSize);
    //UITextElementBase *text2 = text;
    textArray.emplace_back(std::move(text));
}


void UIWindow::addButton(ButtonParams &bParams)
{
    WindowParams wParams = {
        {0.0f, 0.0f},
        bParams.position,
        bParams.bSize,
        fixedToWindow,
        false,
    };

    bParams.position = {0,0};
    addGroup(wParams);
    //std::cout << &groupArray.back() << "\n";
    groupArray.back()->addButton(bParams);
}

void UIWindow::addSlider(SliderParams &sParams)
{
    WindowParams wParams = {
        {0.0f, 0.0f},
        sParams.position,
        sf::Vector2f{sParams.range + sParams.bSize.x, sParams.bSize.y},
        fixedToWindow,
        false,
    };
    //std::cout << sParams.position << "\n";
    addGroup(wParams);
    //sParams.position = {0,0};
    groupArray.back()->addSlider(sParams);
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
        textArray.at(i)->updateElement(window, GUIView, effOrigPos);
        //textArray.at(i)->textWrap(windowBox.getGlobalBounds());
        window.draw(*textArray.at(i));
        //std::cout << textArray.at(i)->getPosition()<< "\n";
    }
    for(unsigned int i=0; i<buttonArray.size(); ++i)
    {
        buttonArray.at(i)->updateElement(window, effOrigPos);
        buttonArray.at(i)->renderButton(window,GUIView);
    }
    for(unsigned int i=0; i<groupArray.size(); ++i)
    {
        groupArray.at(i)->updateElement(window, effOrigPos);
        groupArray.at(i)->renderElements(window, GUIView);
    }
}


void UIWindow::renderWindow(sf::RenderWindow &window, sf::View &GUIView)
{
    effOrigPos = origPosition + sf::Vector2f{window.getSize().x*normPosition.x,
                                             window.getSize().y*normPosition.y};
                                           // std::cout << window.getSize().x << "\n";
    if(fixedToWindow)
    {
        window.setView(GUIView);
        windowBox.setPosition(window.mapPixelToCoords
            (static_cast<sf::Vector2i>(effOrigPos)));
    }
    else
    {
        //window.setView(GUIView);
        //std::cout << "hello\n";
        windowBox.setPosition(effOrigPos);
    }
    window.draw(windowBox);
    renderElements(window, GUIView);
}


UIWindow::UIWindow(WindowParams &params) : normPosition{params.normPosition},
                   origPosition{params.positionOffset}, width{params.wSize.x},
                   height{params.wSize.y}, color{params.color}, fixedToWindow{params.fixedToWin},
                   draggable{params.draggable}
{
    if(!fixedToWindow)
        normPosition = sf::Vector2f{0.0f,0.0f};
    //effOrigPos = sf::Vector2f{normPosition.x*200, normPosition.y*800};
    //origRect
    windowBox.setPosition(effOrigPos);
    windowBox.setSize(sf::Vector2f(width,height));
    windowBox.setFillColor(color);
}

UIWindow::~UIWindow() {}

void UIWindow::checkMouseIntersection(sf::RenderWindow &window)
{
    mouseIntersecting = false;
    resetButtonPair();
    sf::Vector2f mousePosf;

    if(!fixedToWindow)
        mousePosf = window.mapPixelToCoords
                        (sf::Mouse::getPosition(window));
    else
        mousePosf = window.mapPixelToCoords
                        (sf::Mouse::getPosition(window));

    if(windowBox.getGlobalBounds().contains(mousePosf))
    {
        mouseIntersecting = true;
    }
    else
    {
        mouseIntersecting = false;
        //std::cout << "no int\n";
    }

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
    //std::cout << "Button on Click: " << groupIndex << "\n";
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
        //origRect = newRect;
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

bool UIWindow::getFixedToWin()
{
    return fixedToWindow;
}

template void UIWindow::addElement<int>(TextElParams<int> &tParams);
template void UIWindow::addElement<float>(TextElParams<float> &tParams);
template void UIWindow::addElement<bool>(TextElParams<bool> &tParams);
template void UIWindow::addElement<sf::Vector2i>(TextElParams<sf::Vector2i> &tParams);
template void UIWindow::addElement<sf::Vector2f>(TextElParams<sf::Vector2f> &tParams);
template void UIWindow::addElement<Integrators>(TextElParams<Integrators> &tParams);

