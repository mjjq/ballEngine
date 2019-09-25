#include "classKeyBinds.h"

bool KeyBinds::isFuncContinuous = false;

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
    //MAP(F13),
    //MAP(F14),
    //MAP(F15),
    { "LMB", sf::Keyboard::F13 },
    { "MMB", sf::Keyboard::F14 },
    { "RMB", sf::Keyboard::F15 },
    MAP(Pause)
};

#define INVMAP(x) { sf::Keyboard:: x, #x }
KeyStringMap KeyBinds::invKeyMap = {
    INVMAP(A),
    INVMAP(B),
    INVMAP(C),
    INVMAP(D),
    INVMAP(E),
    INVMAP(F),
    INVMAP(G),
    INVMAP(H),
    INVMAP(I),
    INVMAP(J),
    INVMAP(K),
    INVMAP(L),
    INVMAP(M),
    INVMAP(N),
    INVMAP(O),
    INVMAP(P),
    INVMAP(Q),
    INVMAP(R),
    INVMAP(S),
    INVMAP(T),
    INVMAP(U),
    INVMAP(V),
    INVMAP(W),
    INVMAP(X),
    INVMAP(Y),
    INVMAP(Z),
    INVMAP(Num0),
    INVMAP(Num1),
    INVMAP(Num2),
    INVMAP(Num3),
    INVMAP(Num4),
    INVMAP(Num5),
    INVMAP(Num6),
    INVMAP(Num7),
    INVMAP(Num8),
    INVMAP(Num9),
    INVMAP(Escape),
    INVMAP(LControl),
    INVMAP(LShift),
    INVMAP(LAlt),
    INVMAP(LSystem),
    INVMAP(RControl),
    INVMAP(RShift),
    INVMAP(RAlt),
    INVMAP(RSystem),
    INVMAP(Menu),
    INVMAP(LBracket),
    INVMAP(RBracket),
    INVMAP(SemiColon),
    INVMAP(Comma),
    INVMAP(Period),
    INVMAP(Quote),
    INVMAP(Slash),
    INVMAP(BackSlash),
    INVMAP(Tilde),
    INVMAP(Equal),
    INVMAP(Dash),
    INVMAP(Space),
    INVMAP(Return),
    INVMAP(BackSpace),
    INVMAP(Tab),
    INVMAP(PageUp),
    INVMAP(PageDown),
    INVMAP(End),
    INVMAP(Home),
    INVMAP(Insert),
    INVMAP(Delete),
    INVMAP(Add),
    INVMAP(Subtract),
    INVMAP(Multiply),
    INVMAP(Divide),
    INVMAP(Left),
    INVMAP(Right),
    INVMAP(Up),
    INVMAP(Down),
    INVMAP(Numpad0),
    INVMAP(Numpad1),
    INVMAP(Numpad2),
    INVMAP(Numpad3),
    INVMAP(Numpad4),
    INVMAP(Numpad5),
    INVMAP(Numpad6),
    INVMAP(Numpad7),
    INVMAP(Numpad8),
    INVMAP(Numpad9),
    INVMAP(F1),
    INVMAP(F2),
    INVMAP(F3),
    INVMAP(F4),
    INVMAP(F5),
    INVMAP(F6),
    INVMAP(F7),
    INVMAP(F8),
    INVMAP(F9),
    INVMAP(F10),
    INVMAP(F11),
    INVMAP(F12),
    { sf::Keyboard::F13, "LMB" },
    { sf::Keyboard::F14, "MMB" },
    { sf::Keyboard::F15, "RMB" },
    //INVMAP(F13),
    //INVMAP(F14),
    //INVMAP(F15),
    INVMAP(Pause)
};

KeyFuncMap KeyBinds::createMapFromJSON(nlohmann::json &json,
                                       StringFuncMap &sfMap)
{
    //create the initial map mapping array of strings (each representing
    //a keyboard key) to a given function, provided the function sfMap[it.key()]
    //exists.
    ArrStringFuncMap newMap;
    for(auto it = json.begin(); it != json.end(); ++it)
        if(sfMap.find(it.key()) != sfMap.end())
        {
            auto tempArray = json[it.key()].get<std::vector<std::string>>();
            newMap.insert( {tempArray, sfMap[it.key()]} );
        }

    KeyFuncMap finalMap;

    //Converts the array of string represented keys to sf::Keyboard::keys contained
    //within newMap.
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


StringStringMap KeyBinds::stringMapFromJSON(nlohmann::json &json,
                                            StringFuncMap &sfMap)
{
    //create the initial map mapping array of strings (each representing
    //a keyboard key) to a given function, provided the function sfMap[it.key()]
    //exists.
    StringStringMap newMap;
    for(auto it = json.begin(); it != json.end(); ++it)
        if(sfMap.find(it.key()) != sfMap.end())
        {
            bool successfulMap  = true;
            auto tempArray = json[it.key()].get<std::vector<std::string>>();
            std::string stringBind = "";

            unsigned int i=0;
            for(auto &kbKey : tempArray)
            {
                if(keyMap.find(kbKey) != keyMap.end())
                {
                    stringBind += kbKey;
                    if(i<tempArray.size()-1)
                        stringBind += " + ";
                }

                else
                {
                    successfulMap = false;
                    std::cout << kbKey << " doesn't exist in keyMap\n";
                }

                ++i;
            }
            if(successfulMap)
                newMap.insert( {it.key(), stringBind} );
        }

    return newMap;
}


void KeyBinds::loadKeybinds(std::string filePath,
                            std::string sceneType,
                            StringFuncMap &sfMap,
                            KeyFuncMap &keyMap,
                            StringStringMap &stringMap)
{
    using json = nlohmann::json;
    std::ifstream initInput(filePath);
    if(json::accept(initInput))
    {
        std::ifstream input(filePath);
        json j;
        input >> j;
        keyMap = KeyBinds::createMapFromJSON(j["Default"], sfMap);
        KeyFuncMap sceneMap = KeyBinds::createMapFromJSON(j[sceneType], sfMap);
        keyMap.insert(sceneMap.begin(), sceneMap.end());

        stringMap = KeyBinds::stringMapFromJSON(j["Default"], sfMap);
        StringStringMap sceneStringMap = KeyBinds::stringMapFromJSON(j[sceneType], sfMap);
        stringMap.insert(sceneStringMap.begin(), sceneStringMap.end());
    }
}

std::string KeyBinds::to_string(sf::Keyboard::Key &key)
{
    return invKeyMap[key];
}


/**
    Function which handles general key based events. Events are processed provided
    there are no key primary keys held. If these keys are held, the newLayerEvent
    events are processed instead.

    @param &event The event case to process.

    @return Void.
*/
void KeyBinds::keyEvents(sf::Event &event,
                         std::vector<sf::Keyboard::Key > &_pressedKeyStack,
                         std::vector<sf::Keyboard::Key > &_releasedKeyStack)
{
    if(event.type == sf::Event::EventType::KeyPressed)
        _pressedKeyStack.push_back(event.key.code);

    else if(event.type == sf::Event::EventType::MouseButtonPressed)
    {
        switch(event.key.code)
        {
            case(sf::Mouse::Left):
                _pressedKeyStack.push_back(keyMap["LMB"]);
                break;
            case(sf::Mouse::Middle):
                _pressedKeyStack.push_back(keyMap["MMB"]);
                break;
            case(sf::Mouse::Right):
                _pressedKeyStack.push_back(keyMap["RMB"]);
                break;
            default:
                break;
        }
    }

    else if(event.type == sf::Event::EventType::KeyReleased)
    {
        _pressedKeyStack.erase(std::remove(_pressedKeyStack.begin(),
                                          _pressedKeyStack.end(), event.key.code),
                              _pressedKeyStack.end());
        _releasedKeyStack.push_back(event.key.code);
    }

    else if(event.type == sf::Event::EventType::MouseButtonReleased)
    {
        switch(event.key.code)
        {
            case(sf::Mouse::Left):
                _pressedKeyStack.erase(std::remove(_pressedKeyStack.begin(),
                                          _pressedKeyStack.end(), keyMap["LMB"]),
                              _pressedKeyStack.end());
                _releasedKeyStack.push_back(keyMap["LMB"]);
                break;
            case(sf::Mouse::Middle):
                _pressedKeyStack.erase(std::remove(_pressedKeyStack.begin(),
                                          _pressedKeyStack.end(), keyMap["MMB"]),
                              _pressedKeyStack.end());
                _releasedKeyStack.push_back(keyMap["MMB"]);
                break;
            case(sf::Mouse::Right):
                _pressedKeyStack.erase(std::remove(_pressedKeyStack.begin(),
                                          _pressedKeyStack.end(), keyMap["RMB"]),
                              _pressedKeyStack.end());
                _releasedKeyStack.push_back(keyMap["RMB"]);
                break;
            default:
                break;
        }
    }
}

void KeyBinds::exePressedKeys(std::vector<sf::Keyboard::Key > &_pressedKeyStack,
                           KeyFuncMap &_keyBinds)
{
    std::vector<sf::Keyboard::Key > tempStack = _pressedKeyStack;

    /*for(auto & key : tempStack)
    {
        std::cout << to_string(key) << ", ";
    }
    std::cout << "\n";*/

    bool functionFound = false;
    isFuncContinuous = false;
    while(tempStack.size() > 0 && functionFound == false)
    {
        isFuncContinuous = false;
        if(tempStack.size()>1)
        {
            if(_keyBinds.find(tempStack) != _keyBinds.end())
            {
                _keyBinds[tempStack]();
                functionFound = true;
                if(!isFuncContinuous && _pressedKeyStack.size()>0)
                    _pressedKeyStack.pop_back();
            }
            else
            {
                tempStack.pop_back();
            }
        }

        else if(tempStack.size() == 1)
        {
            for(unsigned int i=0; i<_pressedKeyStack.size(); ++i)
            {
                isFuncContinuous = false;
                if(_keyBinds.find({_pressedKeyStack.at(i)}) != _keyBinds.end())
                {
                    _keyBinds[{_pressedKeyStack.at(i)}]();
                    functionFound = true;
                    if(!isFuncContinuous && _pressedKeyStack.size()>0)
                        _pressedKeyStack.erase(_pressedKeyStack.begin() + i);
                }
            }
            tempStack.pop_back();
        }
    }
}

void KeyBinds::exeReleasedKey(std::vector<sf::Keyboard::Key > &_pressedKeyStack,
                               std::vector<sf::Keyboard::Key > &_releasedKey,
                               KeyFuncMap &_releasedKeyBinds)
{
    if(_releasedKey.size()>0)
    {
        std::vector<sf::Keyboard::Key > tempStack = _pressedKeyStack;
        tempStack.push_back(_releasedKey.at(0));

        if(_releasedKeyBinds.find(tempStack) != _releasedKeyBinds.end())
        {
            _releasedKeyBinds[tempStack]();
        }
        else if(_releasedKeyBinds.find({_releasedKey}) != _releasedKeyBinds.end())
        {
            _releasedKeyBinds[{_releasedKey}]();
        }
    }
    _releasedKey.clear();
}
