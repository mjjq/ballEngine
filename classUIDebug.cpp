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
void UIWindow<T>::addElement(std::string font, std::string str, int fontSize, sf::Vector2f position, T *var)
{
    UITextElement<T> text(str, position, fixedToWindow, var);
    currentFont.loadFromFile(font);
    text.setFont(currentFont);
    text.setCharacterSize(fontSize);
    //text.setPosition(position);
    //text.setString(str);
    textArray.push_back(text);
}

template <typename T>
bool UIWindow<T>::ifElementsCollide(sf::Rect<float> rectBound1, sf::Rect<float> rectBound2)
{
    if(rectBound1.intersects(rectBound2))
        return true;
    return false;
}

template <typename T>
void UIWindow<T>::renderElements(sf::RenderWindow &window, sf::View &GUIView)
{
    for(int i=0; i<textArray.size(); i++)
    {

        //if(textArray.at(i).getGlobalBounds().intersects(windowBox.getGlobalBounds()))
        {
            sf::Rect<float> a = textArray.at(0).getGlobalBounds();
            sf::Rect<float> b = windowBox.getGlobalBounds();
            //std::cout << a.top << " " << a.left << " " << a.width << " " << a.height << "\n";
            //std::cout << b.top << " " << b.left << " " << b.width << " " << b.height << "\n";
            textArray.at(i).updateElement(window, GUIView, origPosition);
            window.draw(textArray.at(i));
        }
    }
}

template <typename T>
void UIWindow<T>::renderWindow(sf::RenderWindow &window, sf::View &GUIView)
{
    if(fixedToWindow)
    {
        window.setView(GUIView);
        windowBox.setPosition(window.mapPixelToCoords(static_cast<sf::Vector2i>(origPosition)));
    }
    else
    {
        windowBox.setPosition((origPosition));
    }
    window.draw(windowBox);
    renderElements(window, GUIView);
}

template <typename T>
UIWindow<T>::UIWindow(sf::Vector2f position, float width, float height, bool fixedToWin, sf::Color color) :
            origPosition{position}, width{width}, height{height}, color{color}, fixedToWindow{fixedToWin}
{
    windowBox.setPosition(origPosition);
    windowBox.setSize(sf::Vector2f(width,height));
    windowBox.setFillColor(color);
}

template class UIWindow<int>;
template class UIWindow<float>;
