#include "Character.h"

#include "rt3d.h"


#include "Stun.h"
#include "Poison.h"
#include "ItemUse.h"
#include "Flee.h"
#include "HeavyAttack.h"
#include "LightAttack.h"

void LimitVariable(float &variable, float minValue, float maxValue)
{
	if (variable > maxValue)
		variable = maxValue;
	else if (variable < minValue)
		variable = minValue;
}

void LimitVariable(int &variable, int minValue, int maxValue)
{
	if (variable > maxValue)
		variable = maxValue;
	else if (variable < minValue)
		variable = minValue;
}

Character::Character(string s_name, int s_health, int s_mana, int s_def,int s_str, float s_physRes, float s_fireRes, float s_waterRes, float s_windRes, bool s_player)
{
	name = s_name;
	health = s_health;
	manaPool = s_mana;
	defence = s_def;
	strength = s_str;
	physRes = s_physRes;
	fireRes = s_fireRes;
	waterRes = s_waterRes;
	windRes = s_windRes;
	player = s_player; 

}

void Character::Attack(Character& enemyCharacter)
{
	inCombat = true;

	if (refreshTime <= 0)
	{
		if (queuedAttacks.size() > 0)
		{
			std::list<C_Attack>::iterator it = queuedAttacks.begin();
			std::advance(it, 0);
			enemyCharacter.BeingAttacked(*it);
			refreshTime += it->Refresh();
			manaPool -= it->GetManaCost();
			it = queuedAttacks.erase(it);
		} else {
			int attackOption = rand() % 5 + 1;
			//all classes created quickly for time, will be refined in full game...
			LightAttack defualtAttack_1 = LightAttack();
			HeavyAttack defualtAttack_2 = HeavyAttack();
			Poison defualtAttack_3 = Poison();
			Stun defualtAttack_4 = Stun();

			switch (attackOption)
			{
			case 1:
				enemyCharacter.BeingAttacked(defualtAttack_1);
				refreshTime += defualtAttack_1.Refresh();
				manaPool -= defualtAttack_1.GetManaCost();
				break;
			case 2:
				enemyCharacter.BeingAttacked(defualtAttack_2);
				refreshTime += defualtAttack_2.Refresh();
				manaPool -= defualtAttack_2.GetManaCost();
				break;
			case 3:
				enemyCharacter.BeingAttacked(defualtAttack_3);
				refreshTime += defualtAttack_3.Refresh();
				manaPool -= defualtAttack_3.GetManaCost();
				break;
			case 4:
				enemyCharacter.BeingAttacked(defualtAttack_4);
				refreshTime += defualtAttack_4.Refresh();
				manaPool -= defualtAttack_4.GetManaCost();
				break;
			default:
				break;
			}
	
		}
	}
}

void Character::BeingAttacked(C_Attack s_attack)
{
	s_attack.attackCompleted = false;
	inCombat = true;
	opponentAttack = s_attack;
	if (player)
	{
		//cout << name << " Is being attacked!!! health: " << health << endl;
		cout << "YOU ARE BEING ATTACKED! PRESS " << s_attack.blockingButton << " TO BLOCK ATTACK! (1.UP, 2.DOWN, 3.LEFT, 4.RIGHT)";
	}
}

void Character::Damage(int damageValue)
{
	//cout << damageValue << " damage done to " << name <<   " current health of character: " << health << endl;
	health = health - damageValue;
	if (health <= 0)
		cout << name << " is DEAD!";
}

void Character::Update(float time)
{
	float timeDifference = (time - peviousTime);
	refreshTime -= timeDifference;
	peviousTime = time;
	LimitVariable(refreshTime, 0, 999);

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (inCombat)
	{
		if (player)
		{
			if (opponentAttack.blockingTime >= 0 && opponentAttack.attackCompleted == false)
			{
				opponentAttack.blockingTime -= timeDifference;
				if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT]) // shouldn't these be 1, 2, 3 and 4 according to the output??
				{
					switch (opponentAttack.blockingButton)
					{
					case 1:
						if (keys[SDL_SCANCODE_UP])
							opponentAttack.BlockedAttack(*this, *this);
						else
							opponentAttack.FailedBlockedAttack(*this, *this);
						break;
					case 2:
						if (keys[SDL_SCANCODE_DOWN])
							opponentAttack.BlockedAttack(*this, *this);
						else
							opponentAttack.FailedBlockedAttack(*this, *this);
						break;
					case 3:
						if (keys[SDL_SCANCODE_LEFT])
							opponentAttack.BlockedAttack(*this, *this);
						else
							opponentAttack.FailedBlockedAttack(*this, *this);
						break;
					case 4:
						if (keys[SDL_SCANCODE_RIGHT])
							opponentAttack.BlockedAttack(*this, *this);
						else
							opponentAttack.FailedBlockedAttack(*this, *this);
						break;
					default:
						break;
					}
				}
			} else {
				if (opponentAttack.blockingTime <= 0 && opponentAttack.attackCompleted == false && inCombat == true && player == true)
				{
					opponentAttack.FailedBlockedAttack(*this, *this);
				}
			}
		} else {
			if (opponentAttack.blockingTime >= 0 && opponentAttack.attackCompleted == false)
			{
				if(rand() % 100 >= 70)
					opponentAttack.BlockedAttack(*this, *this);
				else
					opponentAttack.FailedBlockedAttack(*this, *this);
			}
		}
	}

	
}

float Character::ResSelect(int resType)
	{
		float res = 0;
		if (resType == 0) { res = physRes; }
		if (resType == 1) { res = fireRes; }
		if (resType == 2) { res = waterRes; }
		if (resType == 3) { res = windRes; }
		return res;
	}