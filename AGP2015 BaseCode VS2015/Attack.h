
#pragma once
#ifndef C_ATTACK_H
#define C_ATTACK_H

//#include "Character.h"
#include <string>
#include <stdlib.h> 
#include <cstdlib>
#include <iostream>

class Character;

using namespace std;


class C_Attack
{
public:
	float refreshTime = 5;
	int manaCost = 1;
	int minDamage = 1;
	int maxDamage = 1;
	float bleedTime = 1;
	float blockingTime = 3;
	int blockingButton = 1;
	int blockingStatus = 0;
	string attackText = "DEFAULT";
	bool attackCompleted = false;
	int resType; // 0 = normal, 1 = fire, 2 = water, 3 = wind

	//Character combat_Opponent; //Only for prototype - a better system is needed, but for quickness this works atm.

	C_Attack() {};
	virtual void Attack(Character& attacker, Character &opponent, int block);
	//void Attack(NPC attacker);
	virtual void Animation() {};
	virtual void SoundEffect() {};
	void CreateBlockingButton() { blockingButton = rand() % 4 + 1; };
	virtual void BlockedAttack(Character& attacker, Character &opponent);
	virtual void FailedBlockedAttack(Character& attacker, Character &opponent);
	virtual float Refresh() { return refreshTime; }
	virtual int GetManaCost() { return manaCost; }
	virtual float GetBleed() { return bleedTime; }
	virtual string GetAttackName() { return attackText; }
	float damageCalc(Character& a, Character& b); //A is attacker, B is victim
};

#endif