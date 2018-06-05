#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

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
    UIButton *button = new UIButton{font, text, fontSize, func, position, bSize,
                            fixedToWindow, color, changeState};
    //button->addElement<int>(font, text, fontSize, {10.0,10.0});
    buttonArray.emplace_back(button);
}

void UIGroup::addSlider(sf::Vector2f position, float range, float thickness,
                        sf::Vector2f bSize, sf::Vector2f physRange,
                        std::function<void(float)> sliderFunc, float *variable)
{
    UIButton *sliderBox = new UIButton{"", "", 1, [&]{},
                            {bSize.x/2.0f,(bSize.y-thickness)/2.0f}, {range, thickness},
                            fixedToWindow, {200,200,200,255}, false};

    UIButton *sliderClickBox = new UIButton{"", "", 1, [&]{},
                            {0,0}, {range, bSize.y},
                            fixedToWindow, {0,0,0,0}, false};

    UISlider *slider = new UISlider{position, bSize,
                   fixedToWindow, range, {70,70,70,255}, physRange, sliderFunc, variable};

    //sliderBox->setDownFunction([&]{slider->clickIntersectedButton();});
    //sliderBox->setUpFunction([&]{slider->releaseClickedButton();});
    buttonArray.emplace_back(sliderBox);
    buttonArray.emplace_back(sliderClickBox);
    buttonArray.emplace_back(slider);

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
    for(UITextElementBase *textElement : textArray)
    {
        textElement->updateElement(window, GUIView, currPosition);
        textElement->textWrap(windowBox.getGlobalBounds());
        window.draw(*textElement);
    }
    for(UIButton *button : buttonArray)
    {
        button->updateElement(window, currPosition);
        button->renderButton(window,GUIView);
    }
    for(UIGroup *group : groupArray)
    {
        group->updateElement(window, currPosition);
        group->renderElements(window, GUIView);
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

void UIGroup::printButtonAddress()
{
    if(buttonArray.size()>0)
        std::cout << buttonArray.at(1) << "\n";
}
