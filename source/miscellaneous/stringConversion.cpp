#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

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
std::string to_string( const T &value )
{
  std::ostringstream ss;
  ss << value;
  return ss.str();
}

template std::string to_string<sf::Vector2i>(const sf::Vector2i &value);
template std::string to_string<sf::Vector2f>(const sf::Vector2f &value);
template std::string to_string<bool>(const bool &value);
