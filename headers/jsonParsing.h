#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#include <map>
#include "structs.h"
#include "../extern/json.hpp"

using json = nlohmann::json;

namespace beParser
{

bool checkBallJson(json &j, BallSpawnVals &sVals);
bool checkBallGridJson(json &j, BallGridSpawnVals &sVals);
bool checkAABBJson(json &j, AABBSpawnVals &sVals);
bool checkwParamsJson(json &j, WindowParams &wVals);
bool checkBParamsJson(json &j, ButtonParams &bVals,
                      std::map<std::string, std::function<void()>> &funcMap);
bool checkSlParamsJson(json &j, SliderParams &sVals,
                        std::map<std::string,
                        std::pair<std::function<void(float)>, float*>> &funcMap);
bool checkTParamsJson(json &j, TextElBoostParams &tParams);

template <typename T>
TextElParams<T> generateTParams(json &j,
                                   std::map<std::string, T*> &varMap);
template <typename T>
bool checkVMapVariable(std::string str,
                                   std::map<std::string, T*> &varMap);
}

#endif // JSON_PARSING_H
