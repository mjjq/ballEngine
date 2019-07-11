#include "classCharacterManager.h"
#include "Math.h"

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
    if((int)characters.size()>characterIndex && Math::square(direction)>0.0f)
    {
        if(Math::dot(direction, {0.0f, 1.0f}) <= 0)
        {
            if(Math::dot(direction, {1.0f, 0.0f}) > 0.0f)
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

void CharacterManager::setAimAngle(int index, sf::Vector2f targetPos)
{
    if(index < (int)characters.size())
    {
        Character* currChar = characters[index];
        sf::Vector2f relPos = targetPos - currChar->getPosition();

        float angle = 0.0f;
        if(Math::square(relPos) > 0.0f)
        {
            angle = atan2(relPos.y, relPos.x);
        }
        //std::cout << 180.0f * angle / sfVectorMath::PI << "\n";
        currChar->changeAimAngle(180.0f * angle / Math::PI);
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

void CharacterManager::onNotify(Entity& entity, Event event)
{
    switch(event.type)
    {
        case(EventType::New_Character) :
        {
            characters.push_back((Character*)(&entity));
        }
    }
}
