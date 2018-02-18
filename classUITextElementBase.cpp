#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

//#include "classes.h"
//#include <boost/any.hpp>
#include "classTextElementBase.h"
#include "sfVectorMath.h"


void UITextElementBase::textWrap(sf::Rect<float> parentRect)
{
    const sf::String insertString = "\n";
    int a=0;
    for(std::size_t i=0; i<getString().getSize(); i++)
    {
        if(getString()[i] == ' ')
            a = i;
        if(!parentRect.contains(findCharacterPos(i)) && a!=0)
        {
            sf::String currString = getString();
            currString.replace(a, 1, insertString);
            setString(currString);
            a=0;
        }
        else if(!parentRect.contains(findCharacterPos(i)) && a==0)
        {
            sf::String currString = getString();
            currString.insert(i-1, "-" + insertString);
            setString(currString);
        }


    }
}

UITextElementBase::~UITextElementBase() {};

UITextElementBase::UITextElementBase(std::string text, sf::Vector2f position, bool fixedToWin) :
                            initialText{text}, origPosition{position}, fixedToWindow{fixedToWin}

{
    setString(initialText);
    setPosition(position);
    setFillColor(sf::Color::White);
}

