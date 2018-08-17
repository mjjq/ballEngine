#ifndef STRUCTS_H
#define STRUCTS_H
#include <functional>
#include "integrators.h"

typedef std::map<std::string, std::function<void()> > mapstrvoid;
typedef std::map<std::string, std::pair<std::function<void(float)>, float*> > mapstrvoidfloat;


struct WindowParams
{
    sf::Vector2f normPosition;
    sf::Vector2f positionOffset;
    sf::Vector2f wSize;
    bool fixedToWin;
    bool draggable;
    sf::Color color = {50,50,50,150};
    bool isVisible = true;
};

struct TextElParams
{
    std::string font;
    std::string str;
    int fontSize;
    sf::Vector2f position;
    std::function<std::string()> variable = []{return "";};
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
};

struct SliderParams
{
    sf::Vector2f position;
    float range;
    float thickness;
    sf::Vector2f bSize;
    sf::Vector2f physRange = {0,0};
    std::function<void(float)> sliderFunc = [](float i){};
    float *variable = nullptr;
    sf::Color buttonColor = {80,80,80,255};
    sf::Color barColor = {200,200,200,255};
};


struct CompleteWindow
{
    WindowParams wParams;
    std::vector<ButtonParams> bParamsVec;
    std::vector<SliderParams> sParamsVec;
    std::vector<TextElParams> tParamsVec;
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

#endif // STRUCTS_H

