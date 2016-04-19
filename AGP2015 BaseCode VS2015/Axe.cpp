#include "Axe.h"

Axe::Axe(int rarity)
{
	//making bronze (rarity not programmed yet)

	objectName = "Axe";
	name = "Axe";

	switch (rarity) // set prices here for different rarities when set up shop again
	{
	case RARITY_BRONZE:
		attackPower = rand() % 10 + 5;
		speed = 60;
		break;
	case RARITY_SILVER:
		attackPower = rand() % 20 + 15;
		speed = 45;
		break;
	case RARITY_GOLD:
		attackPower = rand() & 41 + 22;
		speed = 32;
		break;
	default:
		attackPower = rand() % 10 + 5; // default bronze
		speed = 60;
		break;
	}
}
