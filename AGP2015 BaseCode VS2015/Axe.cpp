#include "Axe.h"

Axe::Axe(int rarity)
{
	//making bronze (rarity not programmed yet)

	objectName = "Axe";
	rarityOrPotency = rarity;
	name = "Axe";

	switch (rarity) // set prices here for different rarities when set up shop again
	{
	case RARITY_BRONZE:
		attackPower = rand() % 10 + 5;
		speed = 60;
		std::cout << "made bronze axe" << std::endl;
		price = BRONZE_WEAPON_PRICE; 
		break;
	case RARITY_SILVER:
		attackPower = rand() % 20 + 15;
		speed = 45;
		std::cout << "made silver axe" << std::endl;
		price = SILVER_WEAPON_PRICE;
		break;
	case RARITY_GOLD:
		attackPower = rand() & 41 + 22;
		speed = 32;
		std::cout << "made gold axe" << std::endl;
		price = GOLD_WEAPON_PRICE; 
		break;
	default:
		attackPower = rand() % 10 + 5;
		speed = 60;
		std::cout << "made bronze axe" << std::endl;
		price = BRONZE_WEAPON_PRICE;

		break;
	}
}




