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
void UIDebug<T>::addElement(std::string font, std::string str, int fontSize, sf::Vector2f position, T *var)
{
    UITextElement<T> text(str, position, var);
    currentFont.loadFromFile(font);
    text.setFont(currentFont);
    text.setCharacterSize(fontSize);
    //text.setPosition(position);
    //text.setString(str);
    textArray.push_back(text);
}

template <typename T>
void UIDebug<T>::renderElements(sf::RenderWindow &window, sf::View &GUIView)
{
    for(int i=0; i<textArray.size(); i++)
    {
        textArray.at(i).updateElement(window, GUIView);
        window.draw(textArray.at(i));
    }
}

template class UIDebug<int>;
template class UIDebug<float>;
