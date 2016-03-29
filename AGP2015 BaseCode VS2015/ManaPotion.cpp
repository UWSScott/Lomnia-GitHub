#include "ManaPotion.h"
#include "Character.h"

ManaPotion::ManaPotion()
{
	name = "ManaPotion";




	// for now setting all potions to light until i set up potency / rarity for all items. Will be changed :)


	objectName = "Light ManaPotion";
	price = 12.00f;


}

void ManaPotion::Use(Character* character)
{
	//***********************************************
	// Light = restores 30 MP
	// Medium = restores 50 MP
	// Strong = restores 80 MP
	// Checks while adding not to go over max. 
	//************************************************

	cout << " Restoring Mana to player." << endl;

	if (objectName == "Light ManaPotion")
	{
		//add 30mp to player
		Restore(30, character);
	}
	else if (objectName == "Medium ManaPotion")
	{
		//add 50mp to player
		Restore(50, character);
	}
	else if (objectName == "Strong ManaPotion")
	{
		//add 80mp to player
		Restore(80, character);
	}
	else if (objectName == "Holy ManaPotion")
	{
		//restores all mana to player
		character->manaPool = character->max_Mana;
	}


}

void ManaPotion::Restore(int restoreValue, Character* character)
{
	// character max mana stuck at 10, remember to look for it hay 

	int tempCheck = character->manaPool + restoreValue;

	if ((tempCheck > character->max_Mana) || (tempCheck == character->max_Mana))
	{
		character->manaPool = character->max_Mana;
	}
	else
	{
		character->manaPool += restoreValue;
	}


}

