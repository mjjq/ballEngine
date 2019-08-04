#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#include <map>
#include "structs.h"
#include "../extern/json.hpp"
#include "baseObject.h"
#include "classEquipable.h"

namespace beParser
{
using json = nlohmann::json;

json loadJsonFromFile(std::string const & filePath);

bool checkBallJson(json &j, BallSpawnVals &sVals);
bool checkBallGridJson(json &j, BallGridSpawnVals &sVals);
bool checkAABBJson(json &j, AABBSpawnVals &sVals);
bool checkwParamsJson(json &j, WindowParams &wVals);
bool checkBParamsJson(json &j, ButtonParams &bVals,
                      std::map<std::string, std::function<void()>> &funcMap);
bool checkSlParamsJson(json &j, SliderParams &sVals,
                        std::map<std::string,
                        std::pair<std::function<void(float)>, float*>> &funcMap);
bool checkTParamsJson(json &j, TextElParams &tParams,
                                std::map<std::string, std::function<std::string()>> &varMap);
bool checkMaterialParams(json &j, Material& material);
bool checkVertexParams(json &j, std::vector<sf::Vertex > &_vertices);
bool checkObjectPropertyParams(json &j, ObjectProperties& props);
bool checkEquipableParams(json &j, EquipableData& data);

}

#endif // JSON_PARSING_H
