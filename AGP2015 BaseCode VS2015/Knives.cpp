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
		speed = 30;
		break;
	case RARITY_SILVER:
		attackPower = rand() % 15 + 8;
		speed = 18;
		break;
	case RARITY_GOLD:
		attackPower = rand() & 23 + 16;
		speed = 8;
		break;
	default:
		attackPower = rand() % 7 + 3; // default bronze
		speed = 30;
		break;
	}
}