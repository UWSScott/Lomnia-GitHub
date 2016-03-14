#include "Attack.h"
#include "Character.h"

C_Attack::C_Attack(Character* characterRef)
{
	characterReference = characterRef;
}

void C_Attack::SetCharacterReference(Character* characterRef)
{
	characterReference = characterRef;
}


float C_Attack::AttackSpeed(Character* character)
{
	possibleRefresh = refreshTime - ((character->speed / 100) * character->weapon->speed);
	return possibleRefresh;
}


float C_Attack::damageCalc(Character& a, Character& b) //A is attacker, B is victim
{
	float res = b.ResSelect(resType); //Sets the resistance value according to the element of the attack being used (eg. fireRes for fire attacks)
	int damage = (a.strength +  (rand() % maxDamage + minDamage)); //Damage is comprised of both statistic strength and the power of the weapon

	damage = (damage - b.defence)*res;
	if (damage < 1)
		damage = 1;
	cout << attackText << " deals " << damage << " points of damage to " << b.characterName << "! " << endl << endl;
	possibleDamage = damage;
	return damage;
}

void C_Attack::Attack(Character& attacker, Character &opponent, int block)
{
	int damage = (damageCalc(attacker, opponent)/block);
	opponent.Damage(damage);
	cout << damage << " damage done to " << attacker.characterName << " by : " << attackText <<" current health of character: " << attacker.health << endl;
	attackCompleted = true;
}

void C_Attack::FailedBlockedAttack(Character& attacker, Character &opponent)
{
	cout << attacker.characterName << " FAILED BLOCK ATTACK!" << endl;
	blockingTime = 0; 
	blockingStatus = 2;

	Attack(attacker, opponent,  1);
}

void C_Attack::BlockedAttack(Character& attacker, Character &opponent)
{
	cout << attacker.characterName << " BLOCKED ATTACK SUCCESSFULLY!" << endl;
	blockingTime = 0;
	blockingStatus = 1;
	Attack(attacker, opponent, 2);
}