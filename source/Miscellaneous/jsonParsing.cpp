#include <iostream>
#include <map>
#include <fstream>
#include "jsonParsing.h"

nlohmann::json beParser::loadJsonFromFile(std::string const & filePath)
{
    using json = nlohmann::json;
    std::ifstream initInput(filePath);
    if(json::accept(initInput))
    {
        std::ifstream input(filePath);
        json j;
        input >> j;
        return j;
    }

    json j;
    return j;
}

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

bool beParser::checkMaterialParams(json &j, Material& material)
{
    if(j["material"].is_null())
        return false;

    json j2 = j["material"];

    if(!j2["shaderID"].is_null())
    {
    material.shaderID = j2["shaderID"];
    }

    if(!j2["diffuseID"].is_null())
    {
    material.diffuseID = j2["diffuseID"];
    }

    if(!j2["normalID"].is_null())
    {
    material.normalID = j2["normalID"];
    }

    if(!j2["emissionID"].is_null())
    {
    material.emissionID = j2["emissionID"];
    }

    if(!j2["diffuseStrength"].is_null())
    {
    material.diffuseStrength = j2["diffuseStrength"];
    }

    if(!j2["ambientStrength"].is_null())
    {
    material.ambientStrength = j2["ambientStrength"];
    }

    if(!j2["specularStrength"].is_null())
    {
    material.specularStrength = j2["specularStrength"];
    }

    if(!j2["emissionStrength"].is_null())
    {
    material.emissionStrength = j2["emissionStrength"];
    }

    if(!j2["shininess"].is_null())
    {
    material.shininess = j2["shininess"];
    }

    return true;
}

bool beParser::checkVertexParams(json &j, std::vector<sf::Vertex > &_vertices)
{
    if(j["vertices"].is_null())
        return false;

    float scale = 1.0f;
    if(!j["scale"].is_null())
        scale = j["scale"];

    sf::Vertex tempVert;
    for(json& jVerts : j["vertices"])
    {
        if(jVerts["x"].is_null() || jVerts["y"].is_null())
            return false;

        tempVert.position.x = jVerts["x"];
        tempVert.position.y = jVerts["y"];
        tempVert.position *= scale;
        _vertices.push_back(tempVert);
    }

    return true;
}

bool beParser::checkObjectPropertyParams(json &j, ObjectProperties& props)
{
    if(j["objectProperties"].is_null())
        return false;

    json j2 = j["objectProperties"];

    if(!j2["position"].is_null())
    {
        props._position.x = j2["position"]["x"];
        props._position.y = j2["position"]["y"];
    }

    if(!j2["velocity"].is_null())
    {
    props._velocity.x = j2["velocity"]["x"];
    props._velocity.y = j2["velocity"]["y"];
    }

    if(!j2["size"].is_null())
    {
    props._size.x = j2["size"]["x"];
    props._size.y = j2["size"]["y"];
    }

    if(!j2["mass"].is_null())
    {
    props._mass = j2["mass"];
    }

    if(!j2["coefFric"].is_null())
    {
    props._coefFric = j2["coefFric"];
    }

    if(!j2["coefRest"].is_null())
    {
    props._coefRest = j2["coefRest"];
    }

    if(!j2["rotation"].is_null())
    {
    props._rotation = j2["rotation"];
    }

    if(!j2["rotRate"].is_null())
    {
    props._rotRate = j2["rotRate"];
    }

    if(!j2["isStatic"].is_null())
    {
    props._isStatic = j2["isStatic"];
    }

    if(!j2["bullet"].is_null())
    {
    props._bullet = j2["bullet"];
    }

    if(!j2["ignoreGravity"].is_null())
    {
    props._ignoreGravity = j2["ignoreGravity"];
    }

    if(!j2["enableCollisions"].is_null())
    {
    props._enableCollision = j2["enableCollisions"];
    }

    if(!j2["objectType"].is_null())
    {
    props.type = j2["objectType"];
    }

    checkVertexParams(j2, props._vertices);
    checkMaterialParams(j2, props.material);

    return true;
}
