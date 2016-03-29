#include "Axe.h"

Axe::Axe()
{
	//making bronze (rarity not programmed yet)
	attackPower = rand() % 10 + 5; // generates random attack power between 5-9.
	objectName = "Axe";
	name = "Axe";
	speed = 60;
}