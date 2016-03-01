#include "Knives.h"

Knives::Knives() 
{ 
	//making bronze (rarity not programmed yet)
	attackPower = rand() % 7 + 3; // generates random attack power between 3-6.
	objectName = "Knives";
	speed = 30;
}