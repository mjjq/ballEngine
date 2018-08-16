#include <iostream>
#include <map>
#include "../../headers/jsonParsing.h"

bool beParser::checkBallJson(json &j, BallSpawnVals &sVals)
{
    if(j["radius"].is_null())
        return false;
    if(j["mass"].is_null())
        return false;
    if(j["position"].is_null())
        return false;
    if(j["velocity"].is_null())
        return false;

    sVals = {
        j["radius"],
        j["mass"],
        sf::Vector2f({j["position"][0], j["position"][1]}),
        sf::Vector2f({j["velocity"][0], j["velocity"][1]})
    };
    return true;
}

bool beParser::checkBallGridJson(json &j, BallGridSpawnVals &sVals)
{
    if(j["radius"].is_null())
        return false;
    if(j["mass"].is_null())
        return false;
    if(j["position"].is_null())
        return false;
    if(j["velocity"].is_null())
        return false;
    if(j["dimensions"].is_null())
        return false;
    if(j["spacing"].is_null())
        return false;

    sVals = {
        j["radius"],
        j["mass"],
        sf::Vector2f({j["position"][0], j["position"][1]}),
        sf::Vector2f({j["velocity"][0], j["velocity"][1]}),
        sf::Vector2i({j["dimensions"][0], j["dimensions"][1]}),
        j["spacing"]
    };
    return true;
}

bool beParser::checkAABBJson(json &j, AABBSpawnVals &sVals)
{
    if(j["position"].is_null())
        return false;
    if(j["dimensions"].is_null())
        return false;

    sVals = {
        sf::Vector2f({j["position"][0], j["position"][1]}),
        sf::Vector2f({j["dimensions"][0], j["dimensions"][1]})
    };
    return true;
}

bool beParser::checkwParamsJson(json &j, WindowParams &wVals)
{
    if(j["position"].is_null() ||
       j["posoffset"].is_null() ||
       j["dimensions"].is_null() ||
       j["draggable"].is_null() ||
       j["fixedtowindow"].is_null()
       )
        return false;

    if(j["colour"].is_null())
        wVals = {
            sf::Vector2f{j["position"][0], j["position"][1]},
            sf::Vector2f{j["posoffset"][0], j["posoffset"][1]},
            sf::Vector2f{j["dimensions"][0], j["dimensions"][1]},
            j["fixedtowindow"],
            j["draggable"]
        };
    else
        wVals = {
            sf::Vector2f{j["position"][0], j["position"][1]},
            sf::Vector2f{j["posoffset"][0], j["posoffset"][1]},
            sf::Vector2f{j["dimensions"][0], j["dimensions"][1]},
            j["fixedtowindow"],
            j["draggable"],
            sf::Color(j["colour"][0], j["colour"][1], j["colour"][2], j["colour"][3])
        };
    return true;
}

bool beParser::checkBParamsJson(json &j, ButtonParams &bVals,
                                std::map<std::string, std::function<void()>> &funcMap)
{
    if(j["position"].is_null() ||
        j["dimensions"].is_null() ||
        j["textfont"].is_null() ||
        j["fontsize"].is_null() ||
        j["text"].is_null() ||
        j["function"].is_null()
       )
        return false;
    if(funcMap.find(j["function"]) != funcMap.end())
        bVals = {
            j["textfont"],
            j["text"],
            j["fontsize"],
            sf::Vector2f{j["position"][0], j["position"][1]},
            sf::Vector2f{j["dimensions"][0], j["dimensions"][1]},
            funcMap[j["function"]]
        };
    else
        bVals = {
            j["textfont"],
            j["text"],
            j["fontsize"],
            sf::Vector2f{j["position"][0], j["position"][1]},
            sf::Vector2f{j["dimensions"][0], j["dimensions"][1]}
        };
    return true;
}

bool beParser::checkSlParamsJson(json &j, SliderParams &sVals,
                        std::map<std::string,
                        std::pair<std::function<void(float)>, float*>> &funcMap)
{
    if(j["position"].is_null() ||
    j["scrnrange"].is_null() ||
    j["lthickness"].is_null() ||
    j["bsize"].is_null() ||
    j["quanrange"].is_null() ||
    j["function"].is_null()
    )
        return false;

    sVals = {
        sf::Vector2f{j["position"][0], j["position"][1]},
        j["scrnrange"],
        j["lthickness"],
        sf::Vector2f{j["bsize"][0], j["bsize"][1]},
        sf::Vector2f{j["quanrange"][0], j["quanrange"][1]},
        funcMap[j["function"]].first,
        funcMap[j["function"]].second
    };
    return true;
}


bool beParser::checkTParamsJson(json &j, TextElParams &tParams,
                                std::map<std::string, std::function<std::string()>> &varMap)
{
    if(j["position"].is_null() ||
    j["textfont"].is_null() ||
    j["fontsize"].is_null() ||
    j["text"].is_null() ||
    j["variable"].is_null()
       )
        return false;

    if(varMap.find(j["variable"]) != varMap.end())
        tParams = {
            j["textfont"],
            j["text"],
            j["fontsize"],
            sf::Vector2f{j["position"][0], j["position"][1]},
            varMap[j["variable"]]
        };
    else
        tParams = {
            j["textfont"],
            j["text"],
            j["fontsize"],
            sf::Vector2f{j["position"][0], j["position"][1]}
        };
    return true;
}