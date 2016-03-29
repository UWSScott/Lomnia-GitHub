#ifndef MANAPOTION_H
#define MANAPOTION_H
#include "Potion.h"


class ManaPotion : public Potion // hello 
{
public:
	ManaPotion();
	void virtual Use(Character* character);
	void virtual Restore(int restoreValue, Character* character);

};

#endif