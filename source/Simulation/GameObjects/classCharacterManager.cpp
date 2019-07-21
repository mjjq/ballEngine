#include "classCharacterManager.h"
#include "Math.h"

CharacterManager::CharacterManager()
{
    Character::engineNotify.addObserver(this);
}

CharacterManager::~CharacterManager()
{
    Character::engineNotify.removeObserver(this);
}

void CharacterManager::addCharacter(CharacterProperties init)
{
    Character* newChar = new Character{init};
    characters.push_back(std::move(newChar));
}

void CharacterManager::addCharacter(Character* newChar)
{
    characters.push_back(std::move(newChar));
}

void CharacterManager::handleInput(Input input, int characterIndex)
{
    if((int)characters.size()>characterIndex)
        characters[characterIndex]->handleInput(input);
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

void CharacterManager::setTarget(sf::Vector2f targetPos, int characterIndex)
{
    if(characterIndex < (int)characters.size())
    {
        characters[characterIndex]->setTarget(targetPos);
    }
}

void CharacterManager::switchNextItem(int charIndex)
{
    if(charIndex < (int)characters.size())
    {
        characters[charIndex]->switchNextItem();
    }
}

CharacterProperties CharacterManager::getCharacterProperties(int index)
{
    if(index < (int)characters.size())
    {
        return characters[index]->getProperties();
    }

    return CharacterProperties{};
}

void CharacterManager::onNotify(Component& entity, Event event, Container* data)
{
    switch(event.type)
    {
        case(EventType::New_Character) :
        {
            characters.push_back((Character*)(&entity));
            break;
        }
        case(EventType::Delete_Character) :
        {
            Character* character = (Character*)(&entity);
            for(int i=0; i<(int)characters.size(); ++i)
            {
                if(characters[i] == character)
                    characters.erase(characters.begin() + i);
            }
            break;
        }
        default:
            break;
    }
}
