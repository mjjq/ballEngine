#ifndef CLASS_TXTELBASE_H
#define CLASS_TXTELBASE_H

class UITextElementBase : public sf::Text
{
protected:

public:
    virtual ~UITextElementBase();
    UITextElementBase();
    virtual void updateElement(sf::RenderWindow &window, sf::View &GUIView, sf::Vector2f parentPosition) = 0;
    virtual void setOrigPosition(sf::Vector2f newPosition) = 0;
    virtual void textWrap(sf::Rect<float> parentRect) = 0;

};



#endif // CLASS_TXTEL_H
