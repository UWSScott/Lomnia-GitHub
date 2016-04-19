#include "Medkit.h"
#include "Character.h"


Medkit::Medkit(int potency)
{
	name = "HealthPotion";
	switch (potency) 
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

	objectName = "HealthPotion";
	price = 12.00f;

}

void Medkit::Use(Character* character)
{
	
	Restore(restoreValue, character);

	//else if (objectName == "Holy HealthPotion")
	//{
	//	//restores all health to player
	//	character->health = character->max_Health;
	//}

}

void Medkit::Restore(int restoreValue, Character* character)
{
	// character max health stuck at 10,

	int tempCheck = character->health + restoreValue;

	if ((tempCheck > character->max_Health) || (tempCheck == character->max_Health))
	{
		character->health = character->max_Health;
	}
	else
	{
		character->health += restoreValue;
	}


}