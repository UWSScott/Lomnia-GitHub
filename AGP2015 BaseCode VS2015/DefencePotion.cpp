#include "DefencePotion.h"
#include "Character.h"

DefencePotion::DefencePotion()
{
	name = "DefencePotion";
	price = 12.00f;

}

void DefencePotion::Use(Character* character)
{
	cout << " Test: Restoring health to player." << endl;
	//cout << "Bonus defence for a number of rounds - need to work with scott to do this" << endl;

}


void DefencePotion::Restore(int restoreValue, Character* character)
{

	//cout << "Bonus defence for a number of rounds - need to work with scott to do this" << endl;
}
