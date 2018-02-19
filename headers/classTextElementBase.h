#ifndef CLASS_TXTELBASE_H
#define CLASS_TXTELBASE_H

class UITextElementBase : public sf::Text
{
protected:
    std::string initialText;
    std::string wrappedText;
    sf::Font displayFont;
    sf::Vector2f origPosition;

    bool displayElement = true;
    bool fixedToWindow;

public:
    virtual ~UITextElementBase();
    UITextElementBase(std::string text, sf::Vector2f position, bool fixedToWin);
    virtual void updateElement(sf::RenderWindow &window, sf::View &GUIView, sf::Vector2f parentPosition) = 0;
    void textWrap(sf::Rect<float> parentRect);

};



#endif // CLASS_TXTEL_H
