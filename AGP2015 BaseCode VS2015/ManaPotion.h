#ifndef MANAPOTION_H
#define MANAPOTION_H
#include "Potion.h"


class ManaPotion : public Potion // hello 
{
public:
	ManaPotion(int potency);
	void virtual Use(Character* character);
	void virtual Restore(int restoreValue, Character* character);
	int restoreValue; 
};

#endif