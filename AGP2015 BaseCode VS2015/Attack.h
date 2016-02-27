
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
private:
	float possibleDamage = 0;
	float possibleRefresh = 0;
	Character* characterReference;
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
	//float blockingChance = 0;
	int resType; // 0 = normal, 1 = fire, 2 = water, 3 = wind

	//Character combat_Opponent; //Only for prototype - a better system is needed, but for quickness this works atm.

	C_Attack() {};
	C_Attack(Character* characterRef);
	virtual void Attack(Character& attacker, Character &opponent, int block);
	//void Attack(NPC attacker);
	virtual void Animation() {};
	virtual void SoundEffect() {};
	virtual float AttackSpeed(Character* character);
	void CreateBlockingButton() { blockingButton = rand() % 4 + 1; };
	virtual void BlockedAttack(Character& attacker, Character &opponent);
	virtual void FailedBlockedAttack(Character& attacker, Character &opponent);
	virtual float Refresh() { return refreshTime; }
	virtual int GetManaCost() { return manaCost; }
	virtual float GetBleed() { return bleedTime; }
	virtual float GetPossibleDamage() { return possibleDamage; }
	virtual float GetPossibleSpeed() { return possibleRefresh; }
	virtual float GetBalanceValue() { return (possibleDamage / possibleRefresh); }
	virtual string GetAttackName() { return attackText; }
	float damageCalc(Character& a, Character& b); //A is attacker, B is victim
};

#endif