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
    MAP(Escape),
    MAP(LControl),
    MAP(LShift),
    MAP(Z),
    MAP(K),
    MAP(F),
    MAP(C),
    MAP(LAlt),
    MAP(W),
    MAP(A),
    MAP(S),
    MAP(D)
};

KeyFuncMap KeyBinds::createMapFromJSON(nlohmann::json &json, StringFuncMap &sfMap)
{
    //newMap contains map{ 'keybrdkey', []{func();} }
    ArrStringFuncMap newMap;
    for(auto it = json.begin(); it != json.end(); ++it)
        if(sfMap.find(it.key()) != sfMap.end())
        {
            auto tempArray = json[it.key()].get<std::vector<std::string>>();
            newMap.insert( {tempArray, sfMap[it.key()]} );
        }

    KeyFuncMap finalMap;

    for(auto kbArrayIt = newMap.begin(); kbArrayIt != newMap.end(); ++kbArrayIt)
    {
        bool successfulMap = true;
        std::vector<sf::Keyboard::Key> tempArray;
        for(auto &kbKey : kbArrayIt->first)
        {
            if(keyMap.find(kbKey) != keyMap.end())
                tempArray.push_back(keyMap[kbKey]);
            else
            {
                successfulMap = false;
                std::cout << kbKey << " doesn't exist in keyMap\n";
            }
        }
        if(successfulMap)
            finalMap.insert({tempArray, kbArrayIt->second});


    }
    return finalMap;
}
