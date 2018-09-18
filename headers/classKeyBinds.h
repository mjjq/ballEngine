#ifndef CLASS_KEYBINDS_H
#define CLASS_KEYBINDS_H

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include <functional>
#include "../extern/json.hpp"

typedef std::map<sf::Keyboard::Key, std::string> KeyStringMap;
typedef std::map<std::string, sf::Keyboard::Key> StringKeyMap;
typedef std::map<std::string, std::function<void()> > StringFuncMap;
typedef std::map<std::vector<std::string>, std::function<void()> > ArrStringFuncMap;
typedef std::map<std::string, std::string > StringStringMap;
typedef std::map<std::vector<sf::Keyboard::Key>, std::function<void()> > KeyFuncMap;

class KeyBinds
{
    static StringKeyMap keyMap;
public:
    static KeyFuncMap createMapFromJSON(nlohmann::json &json, StringFuncMap &sfMap);
};

#endif // CLASS_KEYBINDS_H
