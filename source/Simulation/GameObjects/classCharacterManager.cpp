#include "classCharacterManager.h"
#include "sfVectorMath.h"

void CharacterManager::addCharacter(CharacterProperties init)
{
    Character* newChar = new Character{init};
    characters.push_back(std::move(newChar));
}

void CharacterManager::addCharacter(Character* newChar)
{
    characters.push_back(std::move(newChar));
}

void CharacterManager::moveCharacter(sf::Vector2f direction, int characterIndex)
{
    if(characters.size()>characterIndex && sfVectorMath::square(direction)>0.0f)
    {
        if(sfVectorMath::dot(direction, {0.0f, 1.0f}) <= 0)
        {
            if(sfVectorMath::dot(direction, {1.0f, 0.0f}) > 0.0f)
                characters[characterIndex]->moveRight();
            else
                characters[characterIndex]->moveLeft();
        }
        else
            characters[characterIndex]->jump();
    }
}

void CharacterManager::equipablePrimary(int charIndex)
{
    if(charIndex < (int)characters.size())
        characters[charIndex]->equipablePrimary();
}

void CharacterManager::newObserver(Observer* obs)
{
    subCharMan.addObserver(obs);
}
