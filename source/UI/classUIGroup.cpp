#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>
#include <memory>

#include "../../headers/classUIGroup.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"
#include "../../headers/classUIButton.h"
#include "../../headers/classUISlider.h"
#include "../../headers/structs.h"

UIGroup::UIGroup(WindowParams &params) : UIWindow(params)
{

}

void UIGroup::addButton(ButtonParams &bParams)
{
    WindowParams wParams = {
        {0.0f, 0.0f},
        bParams.position,
        bParams.bSize,
        fixedToWindow,
        false,
    };
    std::unique_ptr<UIButton> button = std::make_unique<UIButton>(bParams, wParams);
    //button->addElement<int>(font, text, fontSize, {10.0,10.0});
    buttonArray.push_back(std::move(button));
}

void UIGroup::addSlider(SliderParams &sParams)
{
    ButtonParams sliderBoxParams = {
        "",
        "",
        1,
        sf::Vector2f{sParams.bSize.x/2.0f,(sParams.bSize.y-sParams.thickness)/2.0f},
        sf::Vector2f{sParams.range, sParams.thickness},
        std::function<void()>{},
        sParams.barColor,
        false,
    };
    ButtonParams sliderClickBoxParams = {
        "",
        "",
        1,
        sf::Vector2f{0,0},
        sf::Vector2f{sParams.range+sParams.bSize.x, sParams.bSize.y},
        std::function<void()>{},
        sf::Color{0,0,0,0},
        false,
    };
    ButtonParams sliderBParams = {
        "",
        "",
        1,
        sf::Vector2f{0,0},
        sParams.bSize,
        std::function<void()>{},
        sParams.buttonColor,
    };
    WindowParams wParams = {
        {0.0f,0.0f},
        sliderBParams.position,
        sliderBParams.bSize,
        fixedToWindow,
        false,
    };

    addButton(sliderBoxParams);
    addButton(sliderClickBoxParams);
    std::unique_ptr<UISlider> slider = std::make_unique<UISlider>(sParams,
                                                    sliderBParams, wParams);

    buttonArray.emplace_back(std::move(slider));

    buttonArray.at(1)->setDownFunction([&]{buttonArray.at(2)->clickIntersectedButton();});
    buttonArray.at(1)->setUpFunction([&]{buttonArray.at(2)->releaseClickedButton();});
    buttonArray.at(1)->setUpExOverride(true);
}

void UIGroup::updateElement(sf::RenderWindow &window, sf::Vector2f parentPosition)
{
    currPosition = origPosition + parentPosition;
    sf::Rect<float> newRect{currPosition,{width,height}};
    //std::cout << windowBox.getGlobalBounds().left << "\n";
    //origRect = newRect;
    if(fixedToWindow)
        windowBox.setPosition(window.mapPixelToCoords
                             (static_cast<sf::Vector2i>(currPosition)));
    else
        windowBox.setPosition(currPosition);
}

void UIGroup::renderElements(sf::RenderWindow &window, sf::View &GUIView)
{
    for(unsigned int i=0; i<textArray.size(); ++i)
    {
        textArray.at(i)->updateElement(window, GUIView, currPosition);
        textArray.at(i)->textWrap(windowBox.getGlobalBounds());
        window.draw(*textArray.at(i));
    }
    for(unsigned int i=0; i<buttonArray.size(); ++i)
    {
        buttonArray.at(i)->updateElement(window, currPosition);
        buttonArray.at(i)->renderButton(window,GUIView);
    }
    for(unsigned int i=0; i<groupArray.size(); ++i)
    {
        groupArray.at(i)->updateElement(window, currPosition);
        groupArray.at(i)->renderElements(window, GUIView);
    }
}

void UIGroup::setDownFunc(unsigned int buttonIndex, std::function<void()> const& func)
{
    if(buttonIndex >= 0 && buttonIndex < buttonArray.size())
        buttonArray.at(buttonIndex)->setDownFunction(func);
}

UIButton& UIGroup::getButton(unsigned int index)
{

    try{
    if(index >= buttonArray.size())
        throw "Index out of range in UIGroup::getButton().";

    }
    catch(const char* err)
    {
        std::cerr << "Error: " << err << "\n";
    }

    return *buttonArray.at(index);
}

