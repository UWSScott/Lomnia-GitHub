#include "Knives.h"

Knives::Knives(int rarity)
{
	name = "Knives";
	//making bronze (rarity not programmed yet)
	objectName = "Knives";
	rarityOrPotency = rarity; 

	switch (rarity) // set prices here for different rarities when set up shop again
	{
	case RARITY_BRONZE:
		attackPower = rand() % 7 + 3;
		std::cout << "made bronze knives" << std::endl;
		price = BRONZE_WEAPON_PRICE;
		speed = 30;
		
		break;
	case RARITY_SILVER:
		attackPower = rand() % 15 + 8;
		speed = 18;
		price = SILVER_WEAPON_PRICE;
		std::cout << "made silver knives" << std::endl;
		break;
	case RARITY_GOLD:
		attackPower = rand() & 23 + 16;
		speed = 8;
		price = GOLD_WEAPON_PRICE;
		std::cout << "made gold knives" << std::endl;
		break;
	default:
		attackPower = rand() % 7 + 3;
		std::cout << "made bronze knives" << std::endl;
		price = BRONZE_WEAPON_PRICE;
		speed = 30;
		break;
	}
}