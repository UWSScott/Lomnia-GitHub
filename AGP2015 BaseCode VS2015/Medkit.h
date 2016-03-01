#pragma once
#include "Potion.h"


class Medkit : public Potion
{
public:
	Medkit(int potency);
	virtual void use(); 


};