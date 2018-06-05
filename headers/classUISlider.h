#ifndef CLASS_UISLIDER_H
#define CLASS_UISLIDER_H

#include "classUIWindow.h"
#include "classUIButton.h"

class UISlider : public UIButton
{
    //UIButton sliderButton{"./fonts/cour.ttf", "", 12, [&]{}, {0,0}, buttonSize, false, {50,50,50,80}};
    float range;
    float thickness = 2;
    sf::Vector2f currButtonPosition{0,0};
    sf::Vector2f buttonPosOnRel{0,0};
    sf::Vector2f buttonSize{0,0};
    //sf::RectangleShape sliderLine{sf::Vector2f{range,thickness}};
    sf::Color clickedColor = {50,50,50,255};
    sf::Color mouseOverColor = {100,100,100,255};
    sf::Color sliderColor = {150,150,150,255};

    std::function<void(float)> sliderFunc;
    sf::Vector2f physRange;
    float *variable;

    bool sliderDown = false;

public:
    UISlider(sf::Vector2f position, sf::Vector2f bSize,
            bool fixedToWin, float range, sf::Color color,
            sf::Vector2f physRange = {0,0},
            std::function<void(float)> slideFunc = nullptr, float *variable = nullptr);

    void clickIntersectedButton(sf::RenderWindow &window);
    void clickIntersectedButton();
    void releaseClickedButton();
    void renderButton(sf::RenderWindow &window, sf::View &GUIView);
    void updateElement(sf::RenderWindow &window, sf::Vector2f parentPosition);
    void changeOrigin(sf::Vector2i origin);
    void checkMouseIntersection(sf::RenderWindow &window);
};

#endif
