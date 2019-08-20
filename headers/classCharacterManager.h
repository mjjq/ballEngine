#ifndef CLASS_CHAR_MANAGER_H
#define CLASS_CHAR_MANAGER_H

#include <iostream>
#include <vector>
#include "Observer.h"
#include "classCharacter.h"

class CharacterManager : public Observer
{
    Subject subCharMan;
public:
    CharacterManager();
    ~CharacterManager();

    std::vector<Character* > characters;
    void addCharacter(CharacterProperties init);
    void addCharacter(Character* newChar);
    void handleInput(Input input, int characterIndex);
    void newObserver(Observer* obs);
    void setTarget(sf::Vector2f targetPos, int characterIndex);
    CharacterProperties getCharacterProperties(int index);

    virtual void onNotify(Component& entity, Event event, Container* data = nullptr);
};

#endif // CLASS_CHAR_MANAGER_H
