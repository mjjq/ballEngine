#ifndef STRUCTS_H
#define STRUCTS_H
#include <functional>
#include "integrators.h"
#include <boost/variant.hpp>

typedef std::map<std::string, std::function<void()> > mapstrvoid;
typedef std::map<std::string, std::pair<std::function<void(float)>, float*> > mapstrvoidfloat;
typedef boost::variant<int*, float*> boostset;


struct WindowParams
{
    sf::Vector2f normPosition;
    sf::Vector2f positionOffset;
    sf::Vector2f wSize;
    bool fixedToWin;
    bool draggable;
    bool isVisible = true;
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

struct TextElBoostParams
{
    std::string font;
    std::string str;
    int fontSize;
    sf::Vector2f position;
};

struct ButtonParams
{
    std::string font;
    std::string text;
    int fontSize;
    sf::Vector2f position;
    sf::Vector2f bSize;
    std::function<void()> func = []{};
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
    std::vector<TextElParams<int>> tParamsIntVec;
    std::vector<TextElParams<float>> tParamsFloatVec;
    std::vector<TextElParams<bool>> tParamsBoolVec;
    std::vector<TextElParams<sf::Vector2i>> tParams2iVec;
    std::vector<TextElParams<sf::Vector2f>> tParams2fVec;
    std::vector<TextElParams<Integrators>> tParamsIntegVec;
};

struct BallSpawnVals
{
    float radius;
    float mass;
    sf::Vector2f position;
    sf::Vector2f velocity;
};

struct BallGridSpawnVals
{
    float radius;
    float mass;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2i dimensions;
    float spacing;
};

struct AABBSpawnVals
{
    sf::Vector2f position;
    sf::Vector2f dimensions;
};

template struct TextElParams<int>;
#endif // STRUCTS_H

