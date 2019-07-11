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
    std::vector<Character* > characters;
    void addCharacter(CharacterProperties init);
    void addCharacter(Character* newChar);
    void moveCharacter(sf::Vector2f direction, int characterIndex);
    void equipablePrimary(int charIndex);
    void newObserver(Observer* obs);
    void setAimAngle(int index, sf::Vector2f targetPos);
    void switchNextItem(int charIndex);
    CharacterProperties getCharacterProperties(int index);

    virtual void onNotify(Entity& entity, Event event);
};

#endif // CLASS_CHAR_MANAGER_H
