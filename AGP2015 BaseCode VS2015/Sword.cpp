#include "Sword.h"

Sword::Sword()
{
	name = "Sword";
	//making bronze (rarity not programmed yet)
	attackPower = rand() % 8 + 4; // generates random attack power between 4-7. 
	objectName = "Sword";
	speed = 50;
}


