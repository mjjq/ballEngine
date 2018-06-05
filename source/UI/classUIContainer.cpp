#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

#include "../../headers/classUIContainer.h"
#include "../../headers/sfVectorMath.h"


UIContainer::UIContainer()
{

}

void UIContainer::addWindow(sf::Vector2f position, float width, float height, bool fixedToWin, bool draggable, sf::Color color)
{
    UIWindow newWindow{position, width, height, fixedToWin, draggable, color};
    interfaceWindows.push_back(newWindow);
    interfaceWindowIDs.push_back(interfaceWindows.size()-1);
    mouseIntersectionList.push_back(false);
}

void UIContainer::renderWindows(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView)
{
    for(unsigned int i=0; i<interfaceWindows.size(); i++)
    {
        int j = interfaceWindows.size() - 1 - i;
        interfaceWindows.at(j).renderWindow(window, GUIView);
        interfaceWindows.at(j).renderElements(window,GUIView);
        window.setView(originalView);
    }
}

UIWindow &UIContainer::getWindow(unsigned int windowIndex)
{
    try{
    if(windowIndex >= interfaceWindows.size())
        throw "windowIndex out of range in UIContainer::getWindow.";

    }
    catch(const char* err)
    {
        std::cerr << "Error: " << err << "\n";
    }

    return interfaceWindows.at(windowIndex);
}


void UIContainer::checkMouseIntersection(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView)
{
    //window.setView(GUIView);
    for(unsigned int i=0; i<interfaceWindows.size(); i++)
    {
        interfaceWindows.at(i).checkMouseIntersection(window);
        mouseIntersectionList.at(i) = interfaceWindows.at(i).getIsMouseIntersecting();
    }
    //window.setView(originalView);
}

std::pair<bool,int> UIContainer::doesMIntExist()
{
    auto iter = std::find(mouseIntersectionList.begin(), mouseIntersectionList.end(), true);
    if(iter != mouseIntersectionList.end())
    {
        currentIntButton = std::make_pair(true, std::distance(mouseIntersectionList.begin(),iter));
        //return currentIntButton;
    }
    else
        currentIntButton = std::make_pair(false, -1);
    return currentIntButton;
}


void UIContainer::clickOnUI(sf::RenderWindow &window)
{
    currentIntWindow = doesMIntExist();
    bool windowBool = std::get<0>(currentIntWindow);
    int windowIndex = std::get<1>(currentIntWindow);
    if(windowBool)
    {
        getWindow(windowIndex).changeOrigin(window, sf::Mouse::getPosition(window));
        getWindow(windowIndex).clickIntersectedButton(window);
        currentIntButton = getWindow(windowIndex).getClickedButton();
        //std::swap(interfaceWindows.at(windowIndex), interfaceWindows.front());
        //currentIntWindow = doesMIntExist();
        //bool buttonBool = std::get<0>(currentIntButton);
        //int buttonIndex = std::get<1>(currentIntButton);
    }

}


void UIContainer::resetUIClick()
{
    //bool windowBool = std::get<0>(currentIntWindow);
    int windowIndex = std::get<1>(currentIntWindow);
    if(windowIndex != -1)
    {
        //std::cout << windowIndex << "\n";
        getWindow(windowIndex).releaseClickedButton();
        getWindow(windowIndex).resetButtonPair();
    }
    currentIntWindow = std::make_pair<bool,int>(false, -1);
    currentIntButton = std::make_pair<bool,int>(false, -1);
}


bool UIContainer::isWindowDraggable()
{
    if(currentIntWindow.first && !currentIntButton.first)
        return true;
    //std::cout << currentIntButton.first << "\n";
    return false;
}

void UIContainer::dragWindow(sf::RenderWindow &window)
{
   //std::cout << currentIntButton.first << " " << currentIntWindow.first << "\n";
    getWindow(currentIntWindow.second).moveWindow(window, sf::Mouse::getPosition(window));
}

void UIContainer::setViewParameters(sf::RenderWindow &window, sf::View view1, sf::View view2)
{
}
