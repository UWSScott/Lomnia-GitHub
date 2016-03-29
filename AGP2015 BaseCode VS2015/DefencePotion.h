#ifndef DEFENCEPOTION_H
#define DEFENCEPOTION_H
#include "Potion.h"


class DefencePotion : public Potion
{
public:
	DefencePotion();
	void virtual Use(Character* character);
	void virtual Restore(int restoreValue, Character* character);

};

#endif