#ifndef CLASS_CHAR_H
#define CLASS_CHAR_H

#include "classCapsule.h"
#include "classProjectileWeapon.h"
#include "Observer.h"
#include "classInventory.h"

class CharacterState;
enum class Input;

struct ContactData
{
	sf::Vector2f position;
	sf::Vector2f normal;
	sf::Vector2f tangent;
	sf::Vector2f rA;
};

struct CharacterProperties
{
    float movementSpeed = 1.0f;
    float jumpPower = 10.0f;
    float coefFriction = 4.0f;
    float maxHealth = 10.0f;
    float currentHealth = maxHealth;
    bool aimingAtTarget = false;
    sf::Vector2f target = {0.0f, 0.0f};
};

class Character : public Component
{
    static float MAX_SLOPE_ANGLE;
    static float MAX_SLOPE_COSINE;
    CharacterProperties properties;


    PhysicsObject* collider;

    bool slopeOkay = true;
    bool touchingSurface = false;

    Inventory characterItems{};

    CharacterState* currentState;
public:
    void moveSideWays(float input);
    Character(CharacterProperties init);
    ~Character();

    static Subject engineNotify;
    Subject charSubject;

    void setCollider(PhysicsObject* _collider);

    void handleInput(Input input);
    void jump();
    bool updateState();
    bool getSlopeState();

    void equipablePrimary();
    void changeAimAngle(float angle);

    void setHealth(float health);
    float getHealth();
    CharacterProperties getProperties();

    void switchNextItem();

    void setAnimation(std::string const & animationName);

    void setTarget(sf::Vector2f const & target);
};


#include "classCharacterState.h"

#endif // CLASS_CHAR_H
