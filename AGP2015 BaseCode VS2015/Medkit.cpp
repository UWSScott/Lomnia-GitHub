#include "Medkit.h"
#include "Character.h"


Medkit::Medkit(int potency)
{
	// could have different constructor for 'randomly generated' medkit picked up that randomizes which potency is recieved?

	name = "HealthPotion";
	switch (potency)
	{
	case 1:
		objectName = "Light HealthPotion"; // need to include potency
		price = 12.00f;
		break;
	case 2:
		objectName = "Medium HealthPotion";
		price = 20.00f;
		break;
	case 3:
		objectName = "Strong HealthPotion";
		price = 30.00f;
		break;
	case 4:
		objectName = "Holy HealthPotion";
		price = 50.00f;
		break;
	default:
		break;

	}
}

void Medkit::Use(Character* character)
{
	cout << " Restoring health to player." << endl;

	if (objectName == "Light HealthPotion")
	{
		//add 30hp to player
		Restore(30, character);
	}
	else if (objectName == "Medium HealthPotion")
	{
		//add 50hp to player
		character->health += 50;
	}
	else if (objectName == "Strong HealthPotion")
	{
		//add 80hp to player
		Restore(30, character);

	}
	else if (objectName == "Holy HealthPotion")
	{
		//restores all health to player
		character->health = character->max_Health;
	}

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