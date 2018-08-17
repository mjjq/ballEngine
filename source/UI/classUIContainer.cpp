#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>
#include "../../extern/json.hpp"

#include "../../headers/classUIContainer.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/structs.h"
#include "../../headers/jsonParsing.h"


UIContainer::UIContainer(bool _isVisible) :
    canInteract{_isVisible}, isVisible{_isVisible}
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
        newWindow->addButton(compWindow.bParamsVec.at(i));
    for(unsigned int i=0; i<compWindow.sParamsVec.size(); ++i)
        newWindow->addSlider(compWindow.sParamsVec.at(i));
    for(unsigned int i=0; i<compWindow.tParamsVec.size(); ++i)
        newWindow->addElement(compWindow.tParamsVec.at(i));

    interfaceWindows.push_back(std::move(newWindow));
    interfaceWindowIDs.push_back(interfaceWindows.size()-1);
    mouseIntersectionList.push_back(false);
}

void UIContainer::addWindow(json &j,
                            mapstrvoid &bFuncMap,
                            mapstrvoidfloat &sFuncMap,
                            std::map<std::string, std::function<std::string()> > &varMap)
{
    WindowParams wParams;
    if(beParser::checkwParamsJson(j, wParams))
    {
        std::unique_ptr<UIWindow> newWindow = std::make_unique<UIWindow>(wParams);
        for(json &buttonJ : j["Buttons"])
        {
            ButtonParams bParams;
            if(beParser::checkBParamsJson(buttonJ, bParams, bFuncMap))
                newWindow->addButton(bParams);
        }
        for(json &sliderJ : j["Sliders"])
        {
            SliderParams sParams;
            if(beParser::checkSlParamsJson(sliderJ, sParams, sFuncMap))
                newWindow->addSlider(sParams);
        }
        for(json &textJ : j["TextElements"])
        {
            TextElParams tParams;
            if(beParser::checkTParamsJson(textJ, tParams, varMap))
            {
                newWindow->addElement(tParams);
            }
        }
        interfaceWindows.push_back(std::move(newWindow));
        interfaceWindowIDs.push_back(interfaceWindows.size()-1);
        mouseIntersectionList.push_back(false);
    }

}


void UIContainer::renderWindows(sf::RenderWindow &window, sf::View &GUIView, sf::View &originalView)
{
    if(isVisible)
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
    if(canInteract && isVisible)
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

void UIContainer::setWindowIsVisible(int index, bool value)
{
    UIWindow *tempWindow;
    if(index >= 0)
        tempWindow = &getWindow(index);
    else
        tempWindow = &getWindow(interfaceWindows.size()+index);

    tempWindow->setIsVisible(value);
    tempWindow->setCanInteract(value);
    resetUIClick();
}

void UIContainer::setWindowCanInteract(int index, bool value)
{
    UIWindow *tempWindow;
    if(index >= 0)
        tempWindow = &getWindow(index);
    else
        tempWindow = &getWindow(interfaceWindows.size()+index);

    if(isVisible)
    {
        tempWindow->setCanInteract(value);
    }
    resetUIClick();
}

