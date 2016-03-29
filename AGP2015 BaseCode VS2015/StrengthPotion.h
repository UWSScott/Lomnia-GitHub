#ifndef STRENGTHPOTION_H
#define STRENGTHPOTION_H
#include "Potion.h"


class StrengthPotion : public Potion
{


public:
	StrengthPotion();
	void virtual Use(Character* character);
	void virtual Restore(int restoreValue, Character* character);

};

#endif