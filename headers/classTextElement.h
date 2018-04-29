#ifndef CLASS_TXTEL_H
#define CLASS_TXTEL_H
#include "classTextElementBase.h"

template <typename T>
class UITextElement : public UITextElementBase
{
    std::string initialText;
    std::string wrappedText;
    sf::Font displayFont;
    sf::Vector2f origPosition;

    bool displayElement = true;
    bool fixedToWindow;

    T *variable;//function which returns the variable to display
    T displayVariable;
    //Template <typename T>;

public:
    UITextElement(std::string text, sf::Vector2f position, bool fixedToWin, T *var = nullptr, bool wrapText = false,
                                                sf::Rect<float> wrapBounds = sf::Rect<float>(0,0,1000,1000));
    void updateElement(sf::RenderWindow &window, sf::View &GUIView, sf::Vector2f parentPosition);
    void setOrigPosition(sf::Vector2f newPosition);
    void textWrap(sf::Rect<float> parentRect);

};

#endif // CLASS_TXTEL_H
