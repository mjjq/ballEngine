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
UIDebug<T>::UIDebug()
{
    //std::string font = "./fonts/cour.ttf";
    //UITextElement<int> testText(, "mousePosition", {400.0,400.0}, &);
    //textElements.push_back()
}

template <typename T>
void UIDebug<T>::addTextElement(std::string font, std::string text, sf::Vector2f position, T *variable)
{
    textElements.push_back(UITextElement<int>(font,text,position,variable));
    textElements.at(0).updateElement();
}

template <typename T>
void UIDebug<T>::clearTextElements()
{

}

template <typename T>
void UIDebug<T>::moveTextElement(int element)
{

}

template <typename T>
void UIDebug<T>::upAndRenderAllElements(sf::RenderWindow &window)
{
    for(int i=0; i<textElements.size(); i++)
    {
        textElements.at(i).updateElement();
        //window.draw(textElements.at(i));
        std::string a = textElements.at(i).getString();
        sfVectorMath::printVector(textElements.at(i).getPosition());
        std::cout << a << "\n";
    }
}

template <typename T>
UITextElement<T> UIDebug<T>::returnElement(int elementNo)
{
    return textElements.at(elementNo);
}


template class UIDebug<int>;
