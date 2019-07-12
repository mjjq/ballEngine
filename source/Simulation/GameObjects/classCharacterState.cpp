#include "classCharacterState.h"
#include "classCharacter.h"

void WalkState::enterState(Character& character)
{
    character.setAnimation("walk");
}

CharacterState* WalkState::handleInput(Character& character, Input input)
{
    switch(input)
    {
        case(Input::Idle):
            return new IdleState();
    }

    return nullptr;
}

void WalkState::update(Character& character)
{
    character.moveSideWays(direction);
}

void IdleState::enterState(Character& character)
{
    character.setAnimation("idle");
}

CharacterState* IdleState::handleInput(Character& character, Input input)
{
    switch(input)
    {
        case(Input::WalkLeft):
            return new WalkState(-1.0f);
        case(Input::WalkRight):
            return new WalkState(1.0f);

        break;
    }

    return nullptr;
}
