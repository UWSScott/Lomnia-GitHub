#include "SpeedPotion.h"

SpeedPotion::SpeedPotion()
{
	name = "SpeedPotion";
	price = 12.00f;

}

void SpeedPotion::Use(Character* character)
{
	//cout << "Bonus speed for a number of rounds - need to work with scott to do this" << endl;

	//***********************************
	// light = bonus speed for 3 turns
	// medium = bonus speed for 5 turns
	// heavy = bonus speed for 8 turns
	//***********************************


}


void SpeedPotion::Restore(int restoreValue, Character* character)
{

}