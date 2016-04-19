#include "Knives.h"

Knives::Knives(int rarity)
{
	name = "Knives";
	//making bronze (rarity not programmed yet)
	objectName = "Knives";

	switch (rarity) // set prices here for different rarities when set up shop again
	{
	case RARITY_BRONZE:
		attackPower = rand() % 7 + 3;
		std::cout << "made bronze knives" << std::endl;
		speed = 30;
		break;
	case RARITY_SILVER:
		attackPower = rand() % 15 + 8;
		speed = 18;
		std::cout << "made silver knives" << std::endl;
		break;
	case RARITY_GOLD:
		attackPower = rand() & 23 + 16;
		speed = 8;
		std::cout << "made gold knives" << std::endl;
		break;
	default:
		attackPower = rand() % 7 + 3; // default bronze
		speed = 30;
		break;
	}
}