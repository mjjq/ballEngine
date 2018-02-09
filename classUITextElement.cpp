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
void UITextElement<T>::updateElement(sf::RenderWindow &window, sf::View &GUIView)
    {
        if(variable!=nullptr)
        {
            displayVariable = *variable;
            std::cout << variable << " : " << displayVariable << "\n";
            std::string strDispVariable = initialText + " " + std::to_string(displayVariable);
            setString(strDispVariable);
        }
        if(fixedToWindow)
        {
            window.setView(GUIView);
            setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(origPosition)));
        }
    }

template <typename T>
UITextElement<T>::UITextElement(std::string text, sf::Vector2f position, T *var) :
                            initialText{text}, variable{var}, origPosition{position}

{
    setString(initialText);
    setPosition(position);
    setFillColor(sf::Color::White);
}

template class UITextElement<int>;
template class UITextElement<float>;
