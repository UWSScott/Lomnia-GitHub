#include "Sword.h"


Sword::Sword()
{
	//making bronze (rarity not programmed yet)
	attackPower = rand() % 8 + 4; // generates random attack power between 4-7. 
	name = "Sword";
	speed = 50; 
}


	