/**
    @file stringConversion.cpp
    Purpose: Overloads << operator to include cases for handling SFML vectors
    and other SFML objects.

    @author mjjq
*/

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "../../headers/stringConversion.h"
#include "../../headers/integrators.h"

template <typename T>
std::ostream& operator << ( std::ostream& outs, const sf::Vector2<T> &vec2 )
{
  return outs << "(" << vec2.x << ", " << vec2.y << ")";
}

template <typename T>
std::ostream& operator << ( std::ostream& outs, const bool &value )
{
    std::string temp;
    if(value)
        temp = std::string("True");
    else
        temp = std::string("False");
    return outs << temp;
}

template <typename T>
std::ostream &operator << ( std::ostream& outs, const sf::Rect<T> &rect)
{
    return outs << "[(" << rect.left << ", " << rect.top << "), (" << rect.width << ", " << rect.height << ")]";
}

template <typename T>
std::ostream &operator << ( std::ostream& outs, const Integrators &intType)
{
    std::string temp;
    switch(intType)
    {
        case(Integrators::INTEG_EULER):
            temp = std::string("Euler");
        case(Integrators::INTEG_SEM_IMP_EULER):
            temp = std::string("Semi Imp Euler");
        case(Integrators::INTEG_RK4):
            temp = std::string("RK4");
        default:
            temp = std::string("Error, invalid integrator");
    }
    return outs << temp;
}

template <typename T>
std::string to_string( const T &value )
{
  std::ostringstream ss;
  ss << value;
  return ss.str();
}

template std::string to_string<sf::Vector2i>(const sf::Vector2i &value);
template std::string to_string<sf::Vector2f>(const sf::Vector2f &value);
template std::string to_string<sf::Rect<int>>(const sf::Rect<int> &value);
template std::string to_string<sf::Rect<float>>(const sf::Rect<float> &value);
template std::string to_string<bool>(const bool &value);
