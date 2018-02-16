#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

//#include "classes.h"
#include "classTextElement.h"
#include "sfVectorMath.h"


template <typename T>
void UITextElement<T>::updateElement(sf::RenderWindow &window, sf::View &GUIView, sf::Vector2f parentPosition)
    {
        if(variable!=nullptr)
        {
            displayVariable = *variable;
            //std::cout << variable << " : " << displayVariable << "\n";
            std::string strDispVariable = initialText + " " + std::to_string(displayVariable);
            setString(strDispVariable);
        }
        if(fixedToWindow)
        {
            setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(origPosition+parentPosition)));

        }
        else
        {
            setPosition((origPosition+parentPosition));
        }
    }


template <typename T>
void UITextElement<T>::textWrap(sf::Rect<float> parentRect)
{
    //sf::Text tempText = this;
    const sf::String insertString = "\n";
    //std::string currString = initText.getString();
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
           // tempString += "\n";
        }
        else if(!parentRect.contains(findCharacterPos(i)) && a==0)
        {
            sf::String currString = getString();
            currString.insert(i-1, "-" + insertString);
            setString(currString);
        }


    }
    //initText = tempText;
}


template <typename T>
UITextElement<T>::UITextElement(std::string text, sf::Vector2f position, bool fixedToWin, T *var) :
                            initialText{text}, variable{var}, origPosition{position}, fixedToWindow{fixedToWin}

{
    setString(initialText);
    setPosition(position);
    setFillColor(sf::Color::White);
}

template class UITextElement<int>;
template class UITextElement<float>;
