#ifndef POTION_H
#define POTION_H
#include "Item.h"


class Potion : public Item
{
private:
	//enum potency {LIGHT, MEDIUM, HEAVY};
public:
	virtual void Use(Character* character) = 0; //pure virtual function
	void virtual Restore(int restoreValue, Character* character) = 0;
};


#endif