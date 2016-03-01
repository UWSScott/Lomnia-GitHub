#include "ManaPotion.h"

ManaPotion::ManaPotion()
{
	name = "ManaPotion";
	price = 3.00f;
}

void ManaPotion::use()
{
	cout << " Restoring mana to player." << endl;
}
