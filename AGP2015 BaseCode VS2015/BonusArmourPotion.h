#ifndef BONUSARMOURPOTION_H
#define BONUSARMOURPOTION_H
#include "Potion.h"


class BonusArmourPotion : public Potion
{
public:
	BonusArmourPotion();
	void virtual Use(Character* character);
	void virtual Restore(int restoreValue, Character* character);


};

#endif