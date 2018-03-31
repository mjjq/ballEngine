#ifndef STRINGCONV_H
#define STRINGCONV_H

template <typename T>
std::ostream& operator << ( std::ostream& outs, const sf::Vector2<T> &vec2 );

template <typename T>
std::ostream& operator << ( std::ostream& outs, const bool &value );

template <typename T>
std::ostream &operator << ( std::ostream& outs, const sf::Rect<T> &rect);

template <typename T>
std::string to_string( const T& value );

#endif // STRINGCONV_H
