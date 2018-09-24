#ifndef CLASS_KEYBINDS_H
#define CLASS_KEYBINDS_H

#include <iostream>
#include <map>
#include <SFML/Graphics.hpp>
#include <functional>
#include "../extern/json.hpp"
#include <fstream>

typedef std::map<std::string, std::string > StringStringMap;
typedef std::map<std::string, sf::Keyboard::Key> StringKeyMap;
typedef std::map<sf::Keyboard::Key, std::string> KeyStringMap;
typedef std::map<std::string, std::function<void()> > StringFuncMap;
typedef std::map<std::vector<std::string>, std::function<void()> > ArrStringFuncMap;
typedef std::map<std::vector<sf::Keyboard::Key>, std::function<void()> > KeyFuncMap;

class KeyBinds
{
    static StringKeyMap keyMap;
    static KeyStringMap invKeyMap;
public:
    static KeyFuncMap createMapFromJSON(nlohmann::json &json, StringFuncMap &sfMap);
    static StringStringMap stringMapFromJSON(nlohmann::json &json,
                                             StringFuncMap &sfMap);
    static void loadKeybinds(std::string filePath,
                             std::string sceneType,
                             StringFuncMap &sfMap,
                             KeyFuncMap &keyMap,
                             StringStringMap &stringMap);
    static std::string to_string(sf::Keyboard::Key &key);
};

#endif // CLASS_KEYBINDS_H
