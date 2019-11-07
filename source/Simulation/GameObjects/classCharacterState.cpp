#include "classCharacterState.h"
#include "classCharacter.h"

void WalkState::enterState(Character& character)
{
    character.setAnimation("walk");
    character.setMovementSpeed(character.getProperties().walkSpeed);
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
        case(Input::RunLeft):
        {
            return new RunState(-1.0f);
            break;
        }
        case(Input::RunRight):
        {
            return new RunState(1.0f);
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

void RunState::enterState(Character& character)
{
    character.setAnimation("walk");
    character.setMovementSpeed(character.getProperties().runSpeed);
}

CharacterState* RunState::handleInput(Character& character, Input input)
{
    switch(input)
    {
        case(Input::Idle):
            return new IdleState();

        case(Input::WalkLeft):
        {
            if(direction > 0.0f)
                return new IdleState();

            return new WalkState(-1.0f);

            break;
        }
        case(Input::WalkRight):
        {
            if(direction < 0.0f)
                return new IdleState();

            return new WalkState(1.0f);

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

void RunState::update(Character& character)
{
    character.moveSideWays(direction);

    if((direction < 0.0f && !character.getFlippedState()) ||
       direction > 0.0f && character.getFlippedState())
        character.setAnimationSpeed(-2.0f);
    else
        character.setAnimationSpeed(2.0f);
}

void IdleState::enterState(Character& character)
{
    character.setAnimation("idle");
}

void IdleState::update(Character& character)
{
    character.stop('t');
}

CharacterState* IdleState::handleInput(Character& character, Input input)
{
    switch(input)
    {
        case(Input::WalkLeft):
        {
            return new WalkState(-1.0f);
            break;
        }
        case(Input::WalkRight):
        {
            return new WalkState(1.0f);
            break;
        }
        case(Input::RunLeft):
        {
            return new RunState(-1.0f);
            break;
        }
        case(Input::RunRight):
        {
            return new RunState(1.0f);
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
        case(Input::WalkLeft):
        {
            movement = -1.0f;
            character.setMovementSpeed(character.getProperties().walkSpeed);
            break;
        }
        case(Input::WalkRight):
        {
            movement = 1.0f;
            character.setMovementSpeed(character.getProperties().walkSpeed);
            break;
        }
        case(Input::RunLeft):
        {
            movement = -1.0f;
            character.setMovementSpeed(character.getProperties().runSpeed);
            break;
        }
        case(Input::RunRight):
        {
            movement = 1.0f;
            character.setMovementSpeed(character.getProperties().runSpeed);
            break;
        }
        case(Input::Idle):
        {
            movement = 0.0f;
            break;
        }
        case(Input::Fall):
            return new FallState();
        default:
            break;
    }

    return nullptr;
}

void JumpState::update(Character& character)
{
    if(std::abs(movement) > 0.0f)
        character.moveSideWays(movement);
    else
        character.stop('x');
}

void FallState::enterState(Character& character)
{
    character.setAnimation("idle");
}

CharacterState* FallState::handleInput(Character& character, Input input)
{
    switch(input)
    {
        case(Input::WalkLeft):
        {
            movement = -1.0f;
            character.setMovementSpeed(character.getProperties().walkSpeed);
            break;
        }
        case(Input::WalkRight):
        {
            movement = 1.0f;
            character.setMovementSpeed(character.getProperties().walkSpeed);
            break;
        }
        case(Input::RunLeft):
        {
            movement = -1.0f;
            character.setMovementSpeed(character.getProperties().runSpeed);
            break;
        }
        case(Input::RunRight):
        {
            movement = 1.0f;
            character.setMovementSpeed(character.getProperties().runSpeed);
            break;
        }
        case(Input::Idle):
        {
            movement = 0.0f;
            break;
        }
        case(Input::Land):
            return new IdleState();
        default:
            break;
        break;
    }

    return nullptr;
}

void FallState::update(Character& character)
{
    if(std::abs(movement) > 0.0f)
        character.moveSideWays(movement);
    else
        character.stop('x');
}
