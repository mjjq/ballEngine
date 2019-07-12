#ifndef CLASS_CHAR_STATE_H
#define CLASS_CHAR_STATE_H

enum class Input
{
    Idle,
    WalkLeft,
    WalkRight,
    Jump
};

class Character;

class CharacterState
{
public:
    virtual ~CharacterState() {};
    virtual void enterState(Character& character) {};
    virtual CharacterState* handleInput(Character& character, Input input) {};
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

class IdleState : public CharacterState
{
public:

    virtual void enterState(Character& character);

    virtual CharacterState* handleInput(Character& character, Input input);
};

#endif
