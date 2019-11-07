#ifndef CLASS_CHAR_STATE_H
#define CLASS_CHAR_STATE_H

enum class Input
{
    Idle,
    WalkLeft,
    WalkRight,
    RunLeft,
    RunRight,
    Jump,
    Land,
    Fall,
    EnableTarget,
    Equip_Primary,
    DisableTarget
};

class Character;

class CharacterState
{
public:
    virtual ~CharacterState() {};
    virtual void enterState(Character& character) {};
    virtual CharacterState* handleInput(Character& character, Input input) = 0;
    virtual void update(Character& character) {};
};

class WalkState : public CharacterState
{
    float direction;

public:
    WalkState(float _direction) : direction{_direction} {};

    virtual void enterState(Character& character);

    virtual CharacterState* handleInput(Character& character, Input input);

    virtual void update(Character& character);
};

class RunState : public CharacterState
{
    float direction;

public:
    RunState(float _direction) : direction{_direction} {};

    virtual void enterState(Character& character);

    virtual CharacterState* handleInput(Character& character, Input input);

    virtual void update(Character& character);
};

class IdleState : public CharacterState
{
public:

    virtual void enterState(Character& character);

    virtual CharacterState* handleInput(Character& character, Input input);

    virtual void update(Character& character);
};

class JumpState : public CharacterState
{
    float movement = 0.0f;
public:
    virtual void enterState(Character& character);

    virtual CharacterState* handleInput(Character& character, Input input);

    virtual void update(Character& character);
};

class FallState : public CharacterState
{
    float movement = 0.0f;
public:
    virtual void enterState(Character& character);

    virtual CharacterState* handleInput(Character& character, Input input);

    virtual void update(Character& character);
};

#endif
