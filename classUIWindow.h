#ifndef CLASS_UIWIN_H
#define CLASS_UIWIN_H
#include "classTextElement.h"

template <typename T>
class UIWindow
{
    sf::Font currentFont;
    sf::Vector2f origPosition;
    float width;
    float height;
    sf::Color color;
    sf::RectangleShape windowBox;
    std::vector<UITextElement<T>> textArray;

    bool fixedToWindow;

public:
    UIWindow(sf::Vector2f position, float width, float height, bool fixedToWin, sf::Color color = {50,50,50,150});
    void addElement(std::string font, std::string str, int fontSize, sf::Vector2f position, T *var = nullptr);
    bool ifElementsCollide(sf::Rect<float> rectBound1, sf::Rect<float> rectBound2);
    void renderWindow(sf::RenderWindow &window, sf::View &GUIView);
    void renderElements(sf::RenderWindow &window, sf::View &GUIView);
};

#endif // CLASS_UIWIN_H
