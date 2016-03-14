#ifndef MEDKIT_H
#define MEDKIT_H
#include "Potion.h"


class Medkit : public Potion
{
public:
	Medkit(int potency);
	virtual void use(); 


};

#endif