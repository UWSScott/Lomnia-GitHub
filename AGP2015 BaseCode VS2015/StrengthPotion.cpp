#include "StrengthPotion.h"

StrengthPotion::StrengthPotion()
{
	name = "StrengthPotion";
	price = 12.00f;

}

void StrengthPotion::use()
{
	cout << "Bonus strength." << endl;
}
