#include "Sword.h"

Sword::Sword(int rarity)
{
	name = "Sword";
	//making bronze (rarity not programmed yet)
	attackPower = rand() % 8 + 4; // generates random attack power between 4-7. 
	objectName = "Sword";
	speed = 50;


	switch (rarity) // set prices here for different rarities when set up shop again
	{
	case RARITY_BRONZE:
		attackPower = rand() % 8 + 4;
		std::cout << "made bronze sword" << std::endl;
		speed = 50;
		break;
	case RARITY_SILVER:
		attackPower = rand() % 18 + 12;
		speed = 38;
		std::cout << "made silver sword" << std::endl;
		break;
	case RARITY_GOLD:
		attackPower = rand() & 31 + 19;
		speed = 25;
		std::cout << "made gold sword" << std::endl; 
		break;
	default:
		attackPower = rand() % 8 + 4; // default bronze
		speed = 30;
		break;
	}

}


