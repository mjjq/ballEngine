#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

#include "classes.h"
#include "sfVectorMath.h"


template <typename T>
void UITextElement<T>::updateElement()
    {
        displayVariable = *variable;
        std::string strDispVariable = initialText + " " + std::to_string(displayVariable);
        //std::cout << strDispVariable << "\n";
        setString(strDispVariable);
        if(posRelativeToWorld)
        {
        }
    }

template <typename T>
UITextElement<T>::UITextElement(std::string font, std::string text, sf::Vector2f position, T *variable) :
                            initialText{text}, variable{variable}

{
    if(!displayFont.loadFromFile(font))
    {
        std::cout << "Error loading font for " + initialText + " \n";
    }
    setString(initialText);
    setPosition(position);
    setFont(displayFont);
    setFillColor(sf::Color::White);
}

template class UITextElement<int>;
