#pragma once
#ifndef FLEE_H
#define FLEE_H

#include "Attack.h"
class Flee : public C_Attack
{
public:
	Flee() { attackText = "Flee"; 	CreateBlockingButton(); };
	void Attack(){};
};


#endif