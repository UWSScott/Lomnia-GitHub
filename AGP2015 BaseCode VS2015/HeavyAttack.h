#pragma once
#ifndef HEAVYATTACK_H
#define HEAVYATTACK_H

#include "Attack.h"
class HeavyAttack : public C_Attack
{
public:
	HeavyAttack() 
	{ 
		attackText = "Heavy Attack"; 
		minDamage = 6;
		maxDamage = 15;
		manaCost = 0;
		resType = 0;
	};
};


#endif