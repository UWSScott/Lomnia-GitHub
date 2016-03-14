#pragma once
#ifndef POISON_H
#define POISON_H

#include "Attack.h"
class Poison : public C_Attack
{
public:
	Poison() 
	{ 
		attackText = "FIRE"; 	
		CreateBlockingButton(); 
		minDamage = 5;
		maxDamage = 7;
		manaCost = 5;
		resType = 1;
	};

	virtual void DoEffect(float time, Character* character)
	{
		//TODO setup function to poison character
	//	if(rand() % 500 == 0)
		//	character->combatInstance->Damage(rand() % maxDamage);
	}
};


#endif