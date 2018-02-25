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
UITextElement<T>::UITextElement(std::string text, sf::Vector2f position, bool fixedToWin, T *var) :
                            UITextElementBase(text, position, fixedToWin), variable{var}

{

}


template class UITextElement<int>;
//template class UITextElement<const int>;
template class UITextElement<float>;
template class UITextElement<bool>;
template class UITextElement<sf::Vector2i>;
template class UITextElement<sf::Vector2f>;
