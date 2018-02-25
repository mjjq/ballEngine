#ifndef CLASS_BUTTON_H
#define CLASS_BUTTON_H

class UIButton : public UIWindow
{
protected:
    sf::Font displayFont;
    sf::Vector2f origPosition;
    sf::RectangleShape buttonRect;

    bool displayElement = true;
    bool fixedToWindow;

public:
    UIButton(std::string text, sf::Vector2f position, sf::Vector2f bSize, bool fixedToWin);
    //void updateElement(sf::RenderWindow &window, sf::View &GUIView, sf::Vector2f parentPosition);

};



#endif
