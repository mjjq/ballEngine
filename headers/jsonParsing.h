#ifndef JSON_PARSING_H
#define JSON_PARSING_H

#include "structs.h"
#include "../extern/json.hpp"

using json = nlohmann::json;

namespace beParser
{

bool checkBallJson(json &j, BallSpawnVals &sVals);
bool checkBallGridJson(json &j, BallGridSpawnVals &sVals);

}

#endif // JSON_PARSING_H
