#include "../../headers/classKeyBinds.h"

#define MAP(x) { #x, sf::Keyboard:: x }
StringKeyMap KeyBinds::keyMap = {
    MAP(Delete),
    MAP(Comma),
    MAP(Period),
    MAP(Dash),
    MAP(Equal),
    MAP(Num0),
    MAP(P),
    MAP(Space),
    MAP(M),
    MAP(R),
    MAP(Num1),
    MAP(Num2),
    MAP(Escape)
};

KeyFuncMap KeyBinds::createMapFromJSON(nlohmann::json &json, StringFuncMap &sfMap)
{
    StringFuncMap newMap;
    for(auto it = json.begin(); it != json.end(); ++it)
        newMap.insert( {it.value(), sfMap[it.key()]} );

    KeyFuncMap finalMap;

    for(auto it = keyMap.begin(); it != keyMap.end(); ++it)
        finalMap.insert( {it->second, newMap[it->first]} );

    return finalMap;
}
