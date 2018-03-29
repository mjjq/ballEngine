#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

#include "../../headers/classUIContainer.h"
#include "../../headers/sfVectorMath.h"


void UIContainer::addWindow(sf::Vector2f position, float width, float height, bool fixedToWin, bool draggable, sf::Color color)
{
    UIWindow newWindow{position, width, height, fixedToWin, draggable, color};
    interfaceWindows.push_back(newWindow);
    mouseIntersectionList.push_back(false);
}

void UIContainer::renderWindows(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView)
{
    for(int i=0; i<interfaceWindows.size(); i++)
    {
        interfaceWindows.at(i).renderWindow(window, GUIView);
        interfaceWindows.at(i).renderElements(window,GUIView);
        window.setView(originalView);
    }
}

void UIContainer::checkMouseIntersection(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView)
{
    //window.setView(GUIView);
    for(int i=0; i<interfaceWindows.size(); i++)
    {
        interfaceWindows.at(i).checkMouseIntersection(window);
        mouseIntersectionList.at(i) = interfaceWindows.at(i).getIsMouseIntersecting();
    }
    //window.setView(originalView);
}

UIWindow &UIContainer::getWindow(int windowIndex)
{
    try{
    if(windowIndex < 0 || windowIndex >= interfaceWindows.size())
        throw "windowIndex out of range in UIContainer::getWindow.";

    return interfaceWindows.at(windowIndex);

    }
    catch(const char* err)
    {
        std::cerr << "Error: " << err << "\n";
    }
}

std::pair<bool,int> UIContainer::doesMIntExist()
{
    auto iter = std::find(mouseIntersectionList.begin(), mouseIntersectionList.end(), true);
    if(iter != mouseIntersectionList.end())
        return std::make_pair(true, std::distance(mouseIntersectionList.begin(),iter));

    return std::make_pair(false, -1);
}
