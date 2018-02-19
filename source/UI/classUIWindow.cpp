#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <thread>
#include <limits>
#include <tuple>
#include <functional>

#include "../../headers/classUIWindow.h"
#include "../../headers/sfVectorMath.h"

template<class T>
void UIWindow::addElement(std::string font, std::string str, int fontSize, sf::Vector2f position, T *var)
{
    UITextElement<T> *text = new UITextElement<T>{str, position, fixedToWindow, var};
    currentFont.loadFromFile(font);
    text->setFont(currentFont);
    text->setCharacterSize(fontSize);
    //text.setPosition(position);
    //text.setString(str);
    UITextElementBase *text2 = text;
    textArray.emplace_back(text2);
}


bool UIWindow::ifElementsCollide(sf::Rect<float> rectBound1, sf::Rect<float> rectBound2)
{
    if(rectBound1.intersects(rectBound2))
        return true;
    return false;
}


void UIWindow::renderElements(sf::RenderWindow &window, sf::View &GUIView)
{
    for(int i=0; i<textArray.size(); i++)
    {

        //if(textArray.at(i).getGlobalBounds().intersects(windowBox.getGlobalBounds()))
        {
            sf::Rect<float> a = textArray.at(0)->getGlobalBounds();
            sf::Rect<float> b = windowBox.getGlobalBounds();
            //std::cout << a.top << " " << a.left << " " << a.width << " " << a.height << "\n";
            //std::cout << b.top << " " << b.left << " " << b.width << " " << b.height << "\n";
            textArray.at(i)->updateElement(window, GUIView, origPosition);
            textArray.at(i)->textWrap(windowBox.getGlobalBounds());
            window.draw(*textArray.at(i));
        }
    }
}


void UIWindow::renderWindow(sf::RenderWindow &window, sf::View &GUIView)
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


UIWindow::UIWindow(sf::Vector2f position, float width, float height, bool fixedToWin, sf::Color color) :
            origPosition{position}, width{width}, height{height}, color{color}, fixedToWindow{fixedToWin}
{
    windowBox.setPosition(origPosition);
    windowBox.setSize(sf::Vector2f(width,height));
    windowBox.setFillColor(color);
}

template void UIWindow::addElement<int>(std::string font, std::string str, int fontSize, sf::Vector2f position, int *var);
//template void UIWindow::addElement<const int>(std::string font, std::string str, int fontSize, sf::Vector2f position, const int *var);
template void UIWindow::addElement<float>(std::string font, std::string str, int fontSize, sf::Vector2f position, float *var);
template void UIWindow::addElement<sf::Vector2i>(std::string font, std::string str, int fontSize, sf::Vector2f position, sf::Vector2i *var);
template void UIWindow::addElement<sf::Vector2f>(std::string font, std::string str, int fontSize, sf::Vector2f position, sf::Vector2f *var);

