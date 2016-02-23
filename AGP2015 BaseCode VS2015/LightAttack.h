#pragma once
#ifndef LIGHTATTACK_H
#define LIGHTATTACK_H

#include "Attack.h"
class LightAttack : public C_Attack
{
public:
	LightAttack() 
	{
	attackText = "Light Attack"; 
	minDamage = 5;
	maxDamage = 7;
	manaCost = 0;
	resType = 0;
	};
	

};


#endif