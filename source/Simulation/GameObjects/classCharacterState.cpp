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

        case(Input::WalkLeft):
        {
            if(direction > 0.0f)
                return new IdleState();

            break;
        }
        case(Input::WalkRight):
        {
            if(direction < 0.0f)
                return new IdleState();

            break;
        }
        case(Input::Jump):
        {
            return new JumpState();
            break;
        }
        default:
            break;
    }

    return nullptr;
}

void WalkState::update(Character& character)
{
    character.moveSideWays(direction);

    if((direction < 0.0f && !character.getFlippedState()) ||
       direction > 0.0f && character.getFlippedState())
        character.setAnimationSpeed(-1.0f);
    else
        character.setAnimationSpeed(1.0f);
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
        case(Input::Jump):
        {
            return new JumpState();
            break;
        }
        default:
            break;
    }

    return nullptr;
}

void JumpState::enterState(Character& character)
{
    character.setAnimation("jump");
    character.jump();
}

CharacterState* JumpState::handleInput(Character& character, Input input)
{
    switch(input)
    {
        //case(Input::Land):
        //    return new IdleState();
        case(Input::Fall):
            return new FallState();
        default:
            break;
    }

    return nullptr;
}

void FallState::enterState(Character& character)
{
    character.setAnimation("idle");
}

CharacterState* FallState::handleInput(Character& character, Input input)
{
    switch(input)
    {
        case(Input::Land):
            return new IdleState();
        default:
            break;
        break;
    }

    return nullptr;
}
