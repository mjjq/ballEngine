#ifndef CLASS_TXTEL_H
#define CLASS_TXTEL_H

template <typename T>
class UITextElement : public sf::Text
{
    std::string initialText;
    sf::Font displayFont;
    sf::Vector2f origPosition;
    T *variable;//function which returns the variable to display
    T displayVariable;
    //Template <typename T>;

    bool displayElement = true;
    bool fixedToWindow;

public:
    UITextElement(std::string text, sf::Vector2f position, bool fixedToWin, T *var = nullptr);
    void updateElement(sf::RenderWindow &window, sf::View &GUIView, sf::Vector2f parentPosition);

};

#endif // CLASS_TXTEL_H
