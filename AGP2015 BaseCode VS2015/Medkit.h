#ifndef MEDKIT_H
#define MEDKIT_H
#include "Potion.h"


class Medkit : public Potion
{
public:
	Medkit(int potency);
	void virtual Use(Character* character);
	void virtual Restore(int restoreValue, Character* character);
	int restoreValue; 
};

#endif