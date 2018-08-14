#include <iostream>
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
