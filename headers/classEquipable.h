#ifndef CLASS_EQUIPABLE_H
#define CLASS_EQUIPABLE_H

enum class EquipableType
{
    PWeapon,
    MWeapon,
    _Count
};

class Equipable
{
public:
    bool executePrimary = false;
    bool executeSecondary = false;

    virtual EquipableType type() const = 0;
    virtual void primaryFunc() = 0;
    virtual void secondaryFunc() = 0;
};

#endif // CLASS_EQUIPABLE_H
