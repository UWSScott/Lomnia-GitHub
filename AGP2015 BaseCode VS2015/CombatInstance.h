#pragma once

#include "Character.h"
#include "Attack.h"
#include "LightAttack.h"
#include "Flee.h"
#include "HeavyAttack.h"
#include "Poison.h"
#include "Stun.h"
#include "ItemUse.h"
#include <ctime>
class C_Attack;
class ItemUse;
class Character;

class CombatInstance
{
private:
	std::clock_t start;
	double duration = 0;
	double timeDifference = 0;

public:
	float peviousTime = 0;
	CombatInstance() {}
	CombatInstance(Character *s_current, Character *s_opponent);// { currentCharacter = s_current;  opponent = s_opponent; start = std::clock(); currentCharacter->inCombat = true; }

	Character *opponent;
	Character *currentCharacter;
	C_Attack incomingAttack;
	vector<C_Attack*> queuedAttacks = vector<C_Attack*>();
	//list<C_Attack> queuedAttacks = list<C_Attack>();

	void Update();
	void EndCombat();
	void Damage(int damageValue);
	void Attack();
	void BeingAttacked(C_Attack s_attack);
	void Input(C_Attack* selected_Attack);

	bool paused = false;
};
