#include "ManaPotion.h"
#include "Character.h"

ManaPotion::ManaPotion(int potency)
{
	name = "ManaPotion";


	switch (potency) // set prices here for different rarities when set up shop again
	{
	case POTION_POTENCY_LIGHT:
		restoreValue = 30; 
		std::cout << "made light potion" << std::endl;
		break;
	case POTION_POTENCY_MEDIUM:
		restoreValue = 50;
		std::cout << "made medium potion" << std::endl;
		break;
	case POTION_POTENCY_STRONG:
		restoreValue = 80; 
		std::cout << "made strong potion" << std::endl;
		break;
	default:
		restoreValue = 30;
		std::cout << "made light potion" << std::endl;
		break;
	}

	// for now setting all potions to light until i set up potency / rarity for all items. Will be changed :)


	objectName = "ManaPotion";
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
	
	Restore(restoreValue, character);

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

