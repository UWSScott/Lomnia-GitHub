#pragma once
#ifndef STUN_H
#define STUN_H

#include "Attack.h"
class Stun : public C_Attack
{
public:
	Stun() 
	{
		attackText = "Stun";  	
		CreateBlockingButton(); 
		minDamage = 3;
		maxDamage = 4;
		manaCost = 3;
		resType = 0;
	};
};


#endif