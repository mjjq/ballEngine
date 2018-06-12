#ifndef STRUCTS_H
#define STRUCTS_H
#include <functional>

struct WindowParams
{
    sf::Vector2f position;
    sf::Vector2f wSize;
    bool fixedToWin;
    bool draggable;
    sf::Color color = {50,50,50,150};
};

struct TextElBaseParams
{

};

template <typename T>
struct TextElParams : public TextElBaseParams
{
     TextElParams( std::string _font = "",
                   std::string _str = "",
                   int _fontSize = 1,
                   sf::Vector2f _position = {0,0},
                   T *_var = nullptr ) :
                   font{_font}, str{_str}, fontSize{_fontSize},
                   position{_position}, var{_var} {}

    std::string font;
    std::string str;
    int fontSize;
    sf::Vector2f position;
    T *var;// = nullptr;
};

struct ButtonParams
{
    std::string font;
    std::string text;
    int fontSize;
    sf::Vector2f position;
    sf::Vector2f bSize;
    std::function<void()> const& func;
    sf::Color color = {80,80,80,150};
    bool changeState = true;

   /* std::string font, std::string text, int fontSize,
                         sf::Vector2f position, sf::Vector2f bSize,
                         std::function<void()> const& func, sf::Color color,
                         bool changeState

    std::string font, std::string text, int fontSize,
             std::function<void()> const& upFunc, sf::Vector2f position,
             sf::Vector2f bSize, bool fixedToWin, sf::Color color = {80,80,80,150},
             bool changeState = true*/
};

struct SliderParams
{
    sf::Vector2f position;
    float range;
    float thickness;
    sf::Vector2f bSize;
    sf::Vector2f physRange = {0,0};
    std::function<void(float)> sliderFunc = nullptr;
    float *variable = nullptr;
    sf::Color buttonColor = {80,80,80,255};
    sf::Color barColor = {200,200,200,255};
};


struct CompleteWindow
{
    WindowParams wParams;
    std::vector<ButtonParams> bParamsVec;
    std::vector<SliderParams> sParamsVec;
    std::vector<std::unique_ptr<TextElBaseParams>> tParamsVec;
};

template struct TextElParams<int>;
#endif // STRUCTS_H

