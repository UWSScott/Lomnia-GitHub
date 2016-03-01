#include "Medkit.h"

Medkit::Medkit(int potency)
{
	switch (potency)
	{
	case 1:
		objectName = "Light Medkit"; // need to include potency
		price = 12.00f;
		break; 
	case 2:
		objectName = "Medium Medkit";
		price = 20.00f; 
		break; 
	case 3:
		objectName = "Strong Medkit";
		price = 30.00f; 
		break; 
	case 4: 
		objectName = "Holy Medkit";
		price = 50.00f;
		break;
	default: 
		break; 

	}
}

void Medkit::use()
{
	cout << " Restoring health to player." << endl;

	if (objectName == "Light Medkit")
	{
		//add 30hp to player
	}
	else if (objectName == "Medium Medkit")
	{
		//add 50hp to player
	}
	else if (objectName == "Strong Medkit")
	{
		//add 80hp to player
	}
	else if (objectName == "Holy Medkit")
	{
		//restores all health to player
	}

}
