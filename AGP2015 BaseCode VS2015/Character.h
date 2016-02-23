#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <string>
using namespace std;

#include <list>
#include "Attack.h"

/*#include "Stun.h"
#include "Poison.h"
#include "ItemUse.h"
#include "Flee.h"
#include "HeavyAttack.h"
#include "LightAttack.h"*/

class Character
{
private:
	float peviousTime = 0;
public:
	int health = 10;
	int manaPool;
	float refreshTime;
	int strength;
	int speed;
	int defence;
	bool player; //Only for prototype
	bool inCombat; //Only for prototype
	string name = "DEFAULT";
	list<C_Attack> queuedAttacks = list<C_Attack>();
	C_Attack opponentAttack;
	float ResSelect(int resType);
	float physRes;
	float fireRes;
	float waterRes;
	float windRes;

	Character() {};
	Character(string s_name, int s_health, int s_mana, int s_def, int s_str, float s_physRes, float s_fireRes, float s_waterRes, float s_windRes, bool s_player);
	void Attack(Character& enemyCharacter);
	void BeingAttacked(C_Attack s_attack);
	void Update(float time);
	void Damage(int damageValue);
};

#endif