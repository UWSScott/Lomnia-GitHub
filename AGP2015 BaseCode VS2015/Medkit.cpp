#include "Medkit.h"

Medkit::Medkit(int potency)
{
	switch (potency)
	{
	case 1:
		name = "Light Medkit"; // need to include potency
		price = 12.00f;
		break; 
	case 2:
		name = "Medium Medkit";
		price = 20.00f; 
		break; 
	case 3:
		name = "Strong Medkit";
		price = 30.00f; 
		break; 
	case 4: 
		name = "Holy Medkit"; 
		price = 50.00f;
		break;
	default: 
		break; 

	}
}

void Medkit::use()
{
	cout << " Restoring health to player." << endl;

	if (name == "Light Medkit")
	{
		//add 30hp to player
	}
	else if (name == "Medium Medkit")
	{
		//add 50hp to player
	}
	else if (name == "Strong Medkit")
	{
		//add 80hp to player
	}
	else if (name == "Holy Medkit")
	{
		//restores all health to player
	}

}
