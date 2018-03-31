#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

#include "../../headers/classTextElementBase.h"
#include "../../headers/classTextElement.h"
#include "../../headers/sfVectorMath.h"
#include "../../headers/stringConversion.h"


template <typename T>
void UITextElement<T>::updateElement(sf::RenderWindow &window, sf::View &GUIView, sf::Vector2f parentPosition)
{
    //std::cout << variable << " : " << displayVariable << "\n";
        if(variable!=nullptr)
        {
            using namespace std;
            //std::cout << variable << " : " << displayVariable << "\n";
            displayVariable = *variable;
            std::string strDispVariable = initialText + " " + to_string(displayVariable);
            //std::cout << displayVariable << "\n";
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
    //std::cout << static_cast<std::string>(getString());
    const sf::String insertString = "\n";
    int a=0;
    //std::cout << parentRect << "\n";
    for(std::size_t i=0; i<getString().getSize(); i++)
    {
        //std::cout << i << ", " << findCharacterPos(i) << "\n";
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
    //std::cout << wrappedText << "\n";
}

template <typename T>
void UITextElement<T>::setOrigPosition(sf::Vector2f newPosition)
{
    origPosition = newPosition;
}



template <typename T>
UITextElement<T>::UITextElement(std::string text, sf::Vector2f position, bool fixedToWin, T *var, bool wrapText,
                                sf::Rect<float> wrapBounds) : UITextElementBase(), variable{var}, initialText{text},
                                origPosition{position}, fixedToWindow{fixedToWin}

{
    setString(initialText);
    setPosition(position);
    /*if(wrapText)// && wrapBounds != sf::Rect<float>(0,0,0,0))
    {
        std::cout << "Wrapped\n";
        textWrap(wrapBounds);
    }*/
}


template class UITextElement<int>;
//template class UITextElement<const int>;
template class UITextElement<float>;
template class UITextElement<bool>;
template class UITextElement<sf::Vector2i>;
template class UITextElement<sf::Vector2f>;
