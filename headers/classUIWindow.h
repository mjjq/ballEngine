#ifndef CLASS_UIWIN_H
#define CLASS_UIWIN_H

#include "classTextElement.h"
//#include "classUIButton.h"

#include <functional>

class UIButton;

class UIWindow
{
protected:
    sf::Font currentFont;
    sf::Vector2f mouseOffset;

    sf::Vector2f origPosition;
    float width;
    float height;
    sf::Rect<float> origRect{origPosition, {width,height}};

    sf::Color color;
    sf::RectangleShape windowBox;

    std::vector<UITextElementBase*> textArray;
    std::vector<UIButton*> buttonArray;

    bool fixedToWindow = true;
    bool draggable = false;
    bool mouseIntersecting = false;



public:
    UIWindow(sf::Vector2f position, float width, float height, bool fixedToWin, sf::Color color = {50,50,50,150});

    template<class T>
    void addElement(std::string font, std::string str, int fontSize, sf::Vector2f position, T *var = nullptr);

    void addButton(std::string font, std::string text, int fontSize, sf::Vector2f position, sf::Vector2f bSize,
                                                bool fixedToWin, void (*func)(), sf::Color color = {80,80,80,150});

    bool ifElementsCollide(sf::Rect<float> rectBound1, sf::Rect<float> rectBound2);
    void renderWindow(sf::RenderWindow &window, sf::View &GUIView);
    void renderElements(sf::RenderWindow &window, sf::View &GUIView);
    void checkMouseIntersection(sf::RenderWindow &window);
    bool getIsMouseIntersecting();

    void moveWindow(sf::RenderWindow &window, sf::Vector2i newPosition, sf::Vector2i offset = {0,0});
};

#endif // CLASS_UIWIN_H
