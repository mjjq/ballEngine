#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

#include "classUIContainer.h"
#include "sfVectorMath.h"


void UIContainer::addWindow(sf::Vector2f position, float width, float height, bool fixedToWin, sf::Color color)
{
    UIWindow newWindow{position, width, height, fixedToWin, color};
    interfaceWindows.push_back(newWindow);
}

void UIContainer::renderWindows(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView)
{
    for(int i=0; i<interfaceWindows.size(); i++)
    {
        interfaceWindows.at(i).renderWindow(window, GUIView);
        window.setView(originalView);
    }
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
