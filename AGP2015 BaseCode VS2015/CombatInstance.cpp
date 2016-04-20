#include "CombatInstance.h"

CombatInstance::CombatInstance(Character *s_current, Character *s_opponent)
{
	currentCharacter = s_current;  opponent = s_opponent; currentCharacter->inCombat = true; start = 0; //start = std::clock();
}


void CombatInstance::EndCombat()
{
	//if (currentCharacter->isDead() == true)
	//	return;

	if (opponent->isDead())
		currentCharacter->LootEnemy(opponent);

	if (currentCharacter->isDead() || opponent->isDead())
	{
		currentCharacter->CheckQuestGoal(opponent);
		opponent->LeaveCombat();
		currentCharacter->LeaveCombat();
	}


}

void CombatInstance::Update()
{
	if (currentCharacter == NULL || opponent == NULL || currentCharacter->isDead()|| opponent->isDead())
	{
		EndCombat();
		return;
	}

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
	EndCombat();
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

void CombatInstance::Input(C_Attack* selected_Attack)
{
	//if (ItemUse* d = dynamic_cast<ItemUse*>(selected_Attack))
	//{
	//	cout << " GO TFRRDSAJKFLDSJFLASDKFLADSKFLDSK " << endl;
	//}
	queuedAttacks.push_back(selected_Attack);
}

void CombatInstance::Attack()
{
	if (currentCharacter == NULL || currentCharacter->isDead() || opponent == NULL || opponent->isDead())
	{
		///cout << "FSFADSFAFDFASFDSAFDA" << endl;
		return;
	}

	if (currentCharacter->refreshTime <= 0)
	{
		for (auto&& child : queuedAttacks) { cout << " attack: " << typeid(child).name(); }


		cout << "{}{}{}{}" << currentCharacter->characterName << "("<<currentCharacter->health<<")" << " should be attacking! : " << opponent->characterName << " has (attacks) : " << queuedAttacks.size() << endl;

		if (queuedAttacks.size() > 0)
		{
			//std::list<C_Attack>::iterator it = queuedAttacks.begin();
			//std::advance(it, 0);
			C_Attack* currentAttack = queuedAttacks[0];
			if (ItemUse* d = dynamic_cast<ItemUse*>(currentAttack))
			{
				d->Attack(*currentCharacter, *currentCharacter);
			} else {
				cout << " Is not an item use! ? " << endl;
				currentAttack->SetCharacterReference(currentCharacter);
				opponent->combatInstance->BeingAttacked(*currentAttack);
			}
			currentCharacter->refreshTime += currentAttack->Refresh();
			currentCharacter->manaPool -= currentAttack->GetManaCost();
			queuedAttacks.pop_back();
			//it = queuedAttacks.erase(it);
		} else {
			vector<C_Attack> availableAttacks = vector<C_Attack>();
			LightAttack defualtAttack_1 = LightAttack();
			HeavyAttack defualtAttack_2 = HeavyAttack();
			Poison defualtAttack_3 = Poison();
			Stun defualtAttack_4 = Stun();

			defualtAttack_1.SetCharacterReference(currentCharacter);
			defualtAttack_2.SetCharacterReference(currentCharacter);
			defualtAttack_3.SetCharacterReference(currentCharacter);
			defualtAttack_4.SetCharacterReference(currentCharacter);

			availableAttacks.push_back(defualtAttack_1);
			if (currentCharacter->manaPool >= defualtAttack_2.GetManaCost()) { availableAttacks.push_back(defualtAttack_2); }
			if (currentCharacter->manaPool >= defualtAttack_3.GetManaCost()) { availableAttacks.push_back(defualtAttack_3); }
			if (currentCharacter->manaPool >= defualtAttack_4.GetManaCost()) { availableAttacks.push_back(defualtAttack_4); }
			int attackOption = rand() % availableAttacks.size();

			opponent->combatInstance->BeingAttacked(availableAttacks[attackOption]);
			currentCharacter->refreshTime += availableAttacks[attackOption].Refresh();
			currentCharacter->manaPool -= availableAttacks[attackOption].GetManaCost();

			/*switch (attackOption)
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
			}*/

		}

		currentCharacter->manaPool += rand() % 5;
		if (currentCharacter->manaPool < 0) { currentCharacter->manaPool = 0; }
		if (currentCharacter->manaPool > 100) { currentCharacter->manaPool = 100; }
	}
}