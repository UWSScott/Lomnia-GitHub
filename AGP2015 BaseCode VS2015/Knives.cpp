#include "Knives.h"

Knives::Knives()
{
	name = "Knives";
	//making bronze (rarity not programmed yet)
	attackPower = rand() % 7 + 3; // generates random attack power between 3-6.
	objectName = "Knives";
	speed = 30; // so speed potion probably edits this! Somehow, il have to talk to scott 
}

