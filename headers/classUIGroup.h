#ifndef CLASS_UIGROUP_H
#define CLASS_UIGROUP_H

#include "classUIWindow.h"
#include "structs.h"

class UIGroup : public UIWindow
{
    sf::Vector2f currPosition = origPosition;


public:
    UIGroup(WindowParams &params);

    void addButton(ButtonParams &bParams);

    void addSlider(SliderParams &sParams);

    void updateElement(sf::RenderWindow &window, sf::Vector2f parentPosition);

    void renderElements(sf::RenderWindow &window, sf::View &GUIView);

    void setDownFunc(unsigned int buttonIndex, std::function<void()> const& func);
    UIButton& getButton(unsigned int index);
    void printButtonAddress();
};

#endif
