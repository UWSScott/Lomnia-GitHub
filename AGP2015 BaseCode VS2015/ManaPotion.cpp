#include "ManaPotion.h"
#include "Character.h"

ManaPotion::ManaPotion(int potency)
{
	name = "ManaPotion";

	rarityOrPotency = potency;

	switch (potency) // set prices here for different rarities when set up shop again
	{
	case POTION_POTENCY_LIGHT:
		restoreValue = 30; 
		price = 1;
		std::cout << "made m light potion" << std::endl;
		break;
	case POTION_POTENCY_MEDIUM:
		restoreValue = 50;
		std::cout << "made m medium potion" << std::endl;
		price = 2;
		break;
	case POTION_POTENCY_STRONG:
		restoreValue = 80; 
		price = 3;
		std::cout << "made m strong potion" << std::endl;
		break;
	default:
		restoreValue = 30;
		price = LIGHT_POTION_PRICE;
		std::cout << "made light potion" << std::endl;
		break;
	}

	// for now setting all potions to light until i set up potency / rarity for all items. Will be changed :)

	
	objectName = "ManaPotion";
	


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

