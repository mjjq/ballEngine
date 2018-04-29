#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

//#include "classes.h"
//#include <boost/any.hpp>
#include "../../headers/classTextElementBase.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"


/*void UITextElementBase::textWrap(sf::Rect<float> parentRect)
{
    //std::cout << static_cast<std::string>(getString());
    const sf::String insertString = "\n";
    int a=0;
    std::cout << parentRect << "\n";
    for(std::size_t i=0; i<getString().getSize(); i++)
    {
        std::cout << i << ", " << findCharacterPos(i) << "\n";
        if(getString()[i] == ' ')
            a = i;
        if(!parentRect.contains(findCharacterPos(i)) && a!=0)
        {
            sf::String currString = getString();
            currString.replace(a, 1, insertString);
            setString(currString);
            a=0;
        }
        else if(!parentRect.contains(findCharacterPos(i)) && a==0 && i!=0)
        {
            sf::String currString = getString();
            currString.insert(i-1, "-" + insertString);
            setString(currString);
        }
    }
    wrappedText = getString();
    std::cout << wrappedText << "\n";
}

void UITextElementBase::setOrigPosition(sf::Vector2f newPosition)
{
    origPosition = newPosition;
}*/

UITextElementBase::~UITextElementBase() {};

UITextElementBase::UITextElementBase() //:
                            //initialText{text}, origPosition{position}, fixedToWindow{fixedToWin}

{
    /*setString(initialText);
    setPosition(position);*/
    /*if(wrapText && wrapBounds != sf::Rect<float>(0,0,0,0))
    {
        //std::cout << "Wrapped\n";
        textWrap(wrapBounds);
    }*/
//    setFillColor(sf::Color::White);
}

