#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

#include "../../headers/classUIContainer.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/structs.h"


UIContainer::UIContainer()
{

}

/*void UIContainer::addWindow(WindowParams &params)
{
    std::unique_ptr<UIWindow> newWindow = std::make_unique<UIWindow>(params);
    interfaceWindows.push_back(std::move(newWindow));
    interfaceWindowIDs.push_back(interfaceWindows.size()-1);
    mouseIntersectionList.push_back(false);
}*/


void UIContainer::addWindow(CompleteWindow &compWindow)
{

    std::unique_ptr<UIWindow> newWindow = std::make_unique<UIWindow>(compWindow.wParams);

    for(unsigned int i=0; i<compWindow.bParamsVec.size(); ++i)
    {
        newWindow->addButton(compWindow.bParamsVec.at(i));
    }
    for(unsigned int i=0; i<compWindow.sParamsVec.size(); ++i)
        newWindow->addSlider(compWindow.sParamsVec.at(i));
    for(unsigned int i=0; i<compWindow.tParamsIntVec.size(); ++i)
        newWindow->addElement(compWindow.tParamsIntVec.at(i));
    for(unsigned int i=0; i<compWindow.tParamsFloatVec.size(); ++i)
        newWindow->addElement(compWindow.tParamsFloatVec.at(i));
    for(unsigned int i=0; i<compWindow.tParamsBoolVec.size(); ++i)
        newWindow->addElement(compWindow.tParamsBoolVec.at(i));
    for(unsigned int i=0; i<compWindow.tParams2iVec.size(); ++i)
        newWindow->addElement(compWindow.tParams2iVec.at(i));
    for(unsigned int i=0; i<compWindow.tParams2fVec.size(); ++i)
        newWindow->addElement(compWindow.tParams2fVec.at(i));
    for(unsigned int i=0; i<compWindow.tParamsIntegVec.size(); ++i)
        newWindow->addElement(compWindow.tParamsIntegVec.at(i));


    interfaceWindows.push_back(std::move(newWindow));
    interfaceWindowIDs.push_back(interfaceWindows.size()-1);
    mouseIntersectionList.push_back(false);
}

void UIContainer::addTextElType(TextElBaseParams &tParams)
{
    //if(dynamic_cast<TextElParams<int>*>(tParams) != nullptr)
}

void UIContainer::renderWindows(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView)
{
    for(unsigned int i=0; i<interfaceWindows.size(); i++)
    {
        int j = interfaceWindows.size() - 1 - i;
        interfaceWindows.at(j)->renderWindow(window, GUIView);
        //interfaceWindows.at(j)->renderElements(window,GUIView);
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

    return *interfaceWindows.at(windowIndex);
}


void UIContainer::checkMouseIntersection(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView)
{
    for(unsigned int i=0; i<interfaceWindows.size(); i++)
    {
        if(interfaceWindows.at(i)->getFixedToWin())
            window.setView(GUIView);
        interfaceWindows.at(i)->checkMouseIntersection(window);
        mouseIntersectionList.at(i) = interfaceWindows.at(i)->getIsMouseIntersecting();
        window.setView(originalView);
    }
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

void UIContainer::destroyWindow(unsigned int index)
{
    if(index >= 0 && index < interfaceWindows.size())
    {
        interfaceWindows.at(index)->destroyAllElements();
        interfaceWindows.erase(interfaceWindows.begin()+index);
        interfaceWindowIDs.erase(interfaceWindowIDs.begin()+index);
    }
}

void UIContainer::destroyAllWindows()
{
    for(unsigned int i=0; i<interfaceWindows.size(); ++i)
    {
        interfaceWindows.at(i)->destroyAllElements();
    }
    interfaceWindows.clear();
    interfaceWindowIDs.clear();
}

//template void UIContainer::addWindow<int>(CompleteWindow &compWindow);
