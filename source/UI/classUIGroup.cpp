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

UIGroup::UIGroup(sf::Vector2f position, float width, float height, bool fixedToWin,
                 sf::Color color) :
                 UIWindow(position, width, height, fixedToWin, false, color)
{

}

void UIGroup::addButton(std::string font, std::string text, int fontSize,
                        sf::Vector2f position, sf::Vector2f bSize,
                        std::function<void()> const& func, sf::Color color,
                        bool changeState)
{
    std::unique_ptr<UIButton> button = std::make_unique<UIButton>(font, text,
                    fontSize, func, position, bSize, fixedToWindow, color, changeState);
    //button->addElement<int>(font, text, fontSize, {10.0,10.0});
    buttonArray.push_back(std::move(button));
}

void UIGroup::addSlider(sf::Vector2f position, float range, float thickness,
                        sf::Vector2f bSize, sf::Vector2f physRange,
                        std::function<void(float)> sliderFunc, float *variable)
{
    std::unique_ptr<UIButton> sliderBox = std::make_unique<UIButton>("", "", 1,
                            std::function<void()>{}, sf::Vector2f{bSize.x/2.0f,
                            (bSize.y-thickness)/2.0f}, sf::Vector2f{range, thickness},
                            fixedToWindow, sf::Color{200,200,200,255}, false);

    std::unique_ptr<UIButton> sliderClickBox = std::make_unique<UIButton>("", "",
                            1, std::function<void()>{}, sf::Vector2f{0,0},
                            sf::Vector2f{range, bSize.y}, fixedToWindow,
                            sf::Color{0,0,0,0}, false);

    std::unique_ptr<UISlider> slider = std::make_unique<UISlider>(position, bSize,
                            fixedToWindow, range, sf::Color{70,70,70,255}, physRange,
                            sliderFunc, variable);

    //sliderBox->setDownFunction([&]{slider->clickIntersectedButton();});
    //sliderBox->setUpFunction([&]{slider->releaseClickedButton();});
    buttonArray.emplace_back(std::move(sliderBox));
    buttonArray.emplace_back(std::move(sliderClickBox));
    buttonArray.emplace_back(std::move(slider));

    buttonArray.at(1)->setDownFunction([&]{buttonArray.at(2)->clickIntersectedButton();});
    buttonArray.at(1)->setUpFunction([&]{buttonArray.at(2)->releaseClickedButton();});
    buttonArray.at(1)->setUpExOverride(true);
}

void UIGroup::updateElement(sf::RenderWindow &window, sf::Vector2f parentPosition)
{
    currPosition = origPosition + parentPosition;
    sf::Rect<float> newRect{currPosition,{width,height}};
    origRect = newRect;
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

