#pragma once
#include "Item.h"


class Potion : public Item
{
private: 
	//enum potency {LIGHT, MEDIUM, HEAVY};

public: 
	virtual void use() = 0; //pure virtual function

};