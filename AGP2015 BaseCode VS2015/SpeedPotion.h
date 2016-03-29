#ifndef SPEEDPOTION_H
#define SPEEDPOTION_H
#include "Potion.h"


class SpeedPotion : public Potion
{
public:
	SpeedPotion();
	void virtual Use(Character* character);
	void virtual Restore(int restoreValue, Character* character);
};

#endif