#include "CombatInstance.h"

CombatInstance::CombatInstance(Character *s_current, Character *s_opponent)
{
	currentCharacter = s_current;  opponent = s_opponent; currentCharacter->inCombat = true; start = 0; //start = std::clock();
}


void CombatInstance::EndCombat()
{
	if (currentCharacter->isDead() == true)
		return;
	if (opponent->isDead())
		currentCharacter->LootEnemy(opponent);
	currentCharacter->CheckQuestGoal(opponent);


}

void CombatInstance::Update()
{
	//duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	duration = (((std::clock() - start) / (double)CLOCKS_PER_SEC));
	timeDifference = duration - start;
	if (timeDifference >= currentCharacter->refreshTime)
	{
		start = duration;
		currentCharacter->refreshTime = 0;
	}

	//cout << currentCharacter->characterName << " t: " << timeDifference << "start: " << start << " duration: " << duration << endl;

	//if (incomingAttack != NULL)
	incomingAttack.blockingTime -= duration;
	incomingAttack.DoEffect(duration, opponent);
	Attack();
	currentCharacter->BlockAttack();
}

void CombatInstance::BeingAttacked(C_Attack s_attack)
{
	s_attack.attackCompleted = false;
	incomingAttack = s_attack;
	/*if (player)
	{
		//cout << name << " Is being attacked!!! health: " << health << endl;
		cout << "YOU ARE BEING ATTACKED! PRESS " << s_attack.blockingButton << " TO BLOCK ATTACK! (1.UP, 2.DOWN, 3.LEFT, 4.RIGHT)";
	}*/
}

void CombatInstance::Damage(int damageValue)
{
	//cout << damageValue << " damage done to " << name <<   " current health of character: " << health << endl;
	currentCharacter->health = (currentCharacter->health - damageValue);
	if (currentCharacter->isDead() == true)
	{
		cout << currentCharacter->characterName << " is DEAD!";
		delete opponent->combatInstance;
	}
}

void CombatInstance::Input(C_Attack selected_Attack)
{
	queuedAttacks.push_back(selected_Attack);
}

void CombatInstance::Attack()
{
	if (currentCharacter->refreshTime <= 0)
	{
		cout << currentCharacter->characterName << "("<<currentCharacter->health<<")" << " should be attacking! : " << opponent->characterName << " has (attacks) : " << queuedAttacks.size() << endl;

		if (queuedAttacks.size() > 0)
		{
			std::list<C_Attack>::iterator it = queuedAttacks.begin();
			std::advance(it, 0);
			it->SetCharacterReference(currentCharacter);
			opponent->combatInstance->BeingAttacked(*it);
			currentCharacter->refreshTime += it->Refresh();
			currentCharacter->manaPool -= it->GetManaCost();
			it = queuedAttacks.erase(it);
		} else {
			int attackOption = rand() % 5 + 1;
			LightAttack defualtAttack_1 = LightAttack();
			HeavyAttack defualtAttack_2 = HeavyAttack();
			Poison defualtAttack_3 = Poison();
			Stun defualtAttack_4 = Stun();

			defualtAttack_1.SetCharacterReference(currentCharacter);
			defualtAttack_2.SetCharacterReference(currentCharacter);
			defualtAttack_3.SetCharacterReference(currentCharacter);
			defualtAttack_4.SetCharacterReference(currentCharacter);

			switch (attackOption)
			{
			case 1:
				opponent->combatInstance->BeingAttacked(defualtAttack_1);
				currentCharacter->refreshTime += defualtAttack_1.Refresh();
				currentCharacter->manaPool -= defualtAttack_1.GetManaCost();
				break;
			case 2:
				opponent->combatInstance->BeingAttacked(defualtAttack_2);
				currentCharacter->refreshTime += defualtAttack_2.Refresh();
				currentCharacter->manaPool -= defualtAttack_2.GetManaCost();
				break;
			case 3:
				opponent->combatInstance->BeingAttacked(defualtAttack_3);
				currentCharacter->refreshTime += defualtAttack_3.Refresh();
				currentCharacter->manaPool -= defualtAttack_3.GetManaCost();
				break;
			case 4:
				opponent->combatInstance->BeingAttacked(defualtAttack_4);
				currentCharacter->refreshTime += defualtAttack_4.Refresh();
				currentCharacter->manaPool -= defualtAttack_4.GetManaCost();
				break;
			default:
				break;
			}

		}
	}
}