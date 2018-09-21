#include "../../headers/classKeyBinds.h"

#define MAP(x) { #x, sf::Keyboard:: x }
StringKeyMap KeyBinds::keyMap = {
    MAP(A),
MAP(B),
MAP(C),
MAP(D),
MAP(E),
MAP(F),
MAP(G),
MAP(H),
MAP(I),
MAP(J),
MAP(K),
MAP(L),
MAP(M),
MAP(N),
MAP(O),
MAP(P),
MAP(Q),
MAP(R),
MAP(S),
MAP(T),
MAP(U),
MAP(V),
MAP(W),
MAP(X),
MAP(Y),
MAP(Z),
MAP(Num0),
MAP(Num1),
MAP(Num2),
MAP(Num3),
MAP(Num4),
MAP(Num5),
MAP(Num6),
MAP(Num7),
MAP(Num8),
MAP(Num9),
MAP(Escape),
MAP(LControl),
MAP(LShift),
MAP(LAlt),
MAP(LSystem),
MAP(RControl),
MAP(RShift),
MAP(RAlt),
MAP(RSystem),
MAP(Menu),
MAP(LBracket),
MAP(RBracket),
MAP(SemiColon),
MAP(Comma),
MAP(Period),
MAP(Quote),
MAP(Slash),
MAP(BackSlash),
MAP(Tilde),
MAP(Equal),
MAP(Dash),
MAP(Space),
MAP(Return),
MAP(BackSpace),
MAP(Tab),
MAP(PageUp),
MAP(PageDown),
MAP(End),
MAP(Home),
MAP(Insert),
MAP(Delete),
MAP(Add),
MAP(Subtract),
MAP(Multiply),
MAP(Divide),
MAP(Left),
MAP(Right),
MAP(Up),
MAP(Down),
MAP(Numpad0),
MAP(Numpad1),
MAP(Numpad2),
MAP(Numpad3),
MAP(Numpad4),
MAP(Numpad5),
MAP(Numpad6),
MAP(Numpad7),
MAP(Numpad8),
MAP(Numpad9),
MAP(F1),
MAP(F2),
MAP(F3),
MAP(F4),
MAP(F5),
MAP(F6),
MAP(F7),
MAP(F8),
MAP(F9),
MAP(F10),
MAP(F11),
MAP(F12),
MAP(F13),
MAP(F14),
MAP(F15),
MAP(Pause)
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

void KeyBinds::loadKeybinds(std::string filePath, std::string sceneType,
                            StringFuncMap &sfMap, KeyFuncMap &keyMap)
{
    using json = nlohmann::json;
    std::ifstream initInput(filePath);
    if(json::accept(initInput))
    {
        std::ifstream input(filePath);
        json j;
        input >> j;
        keyMap = KeyBinds::createMapFromJSON(j[sceneType], sfMap);
    }
}
