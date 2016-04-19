#include "Inventory.h"
#include <stdlib.h>
#include <ctime>

Inventory::Inventory()
{
	//for (int i = 0; i < 5; i++) //Arnould starts with 5 potions
	//{
	//	Potion* tempPotion = new Medkit(1);
	//	items.push_back(tempPotion);
	//}
	menuUI = new UI;
	gold = 500.00f;
}

void Inventory::addItem(string itemNameID)
{
	// find out what item to add, create item, push onto inventory
	if (itemNameID == "HealthPotion")
	{
		Potion* tempPotion = new Medkit(1);
		items.push_back(tempPotion);
	}
	else if (itemNameID == "ManaPotion")
	{
		Potion* tempPotion = new ManaPotion();
		items.push_back(tempPotion);
	}
	else if (itemNameID == "BonusArmourPotion")
	{
		Potion* tempPotion = new BonusArmourPotion();
		items.push_back(tempPotion);
	}
	else if (itemNameID == "DefencePotion")
	{
		Potion* tempPotion = new DefencePotion();
		items.push_back(tempPotion);
	}
	else if (itemNameID == "SpeedPotion")
	{
		Potion* tempPotion = new SpeedPotion();
		items.push_back(tempPotion);
	}
	else if (itemNameID == "StrengthPotion")
	{
		Potion* tempPotion = new StrengthPotion();
		items.push_back(tempPotion);
	}
	else if (itemNameID == "Sword")
	{
		Sword* tempSword = new Sword(1);
		items.push_back(tempSword);
	}
	else if (itemNameID == "Axe")
	{
		Axe* tempAxe = new Axe(1);
		items.push_back(tempAxe);
	}
	else if (itemNameID == "Knives")
	{
		Knives* tempKnives = new Knives(1);
		items.push_back(tempKnives);
	}
}

void Inventory::removeItem(string itemNameID)
{
	bool itemFound = false;

	for (iter = items.begin(); iter != items.end(); iter++)
	{
		if ((**iter).name == itemNameID)
		{
			items.erase(iter);
			itemFound = true;
			break;

		}
	}

	if (itemFound == false)
	{
		cout << "Stop trying to break my test program by removing items that you don't have." << endl;
	}
}

void Inventory::buyItem(string itemNameID, int rarity)
{
	int playerChoice;
	int price; 

	switch (rarity)
	{
	case 1: 
		//bronze weapon price
		price = BRONZE_WEAPON_PRICE;
		break;
	case 2: 
		//silver
		price = SILVER_WEAPON_PRICE;
		break; 
	case 3: 
		price = GOLD_WEAPON_PRICE;
		//gold
		break; 
	}

	if (price > gold)
	{
		cout << "Sorry, you don't have enough gold to buy a " << itemNameID << endl;
	}
	else
	{
		if (itemNameID == "HealthPotion")
		{
	/*		cout << "What potency of HealthPotion would you like to buy? " << endl;
			cout << "1. Light" << endl << "2. Medium" << endl << "3. Heavy" << endl << "4. Holy";
			cin >> playerChoice;
			Potion* tempPotion = new Medkit(playerChoice);
			items.push_back(tempPotion);*/

			Potion* tempPotion = new Medkit(1);
			items.push_back(tempPotion);
		}
		else if (itemNameID == "ManaPotion")
		{
			Potion* tempPotion = new ManaPotion();
			items.push_back(tempPotion);
		}
		else if (itemNameID == "BonusArmourPotion")
		{
			Potion* tempPotion = new BonusArmourPotion();
			items.push_back(tempPotion);
		}
		else if (itemNameID == "DefencePotion")
		{
			Potion* tempPotion = new DefencePotion();
			items.push_back(tempPotion);
		}
		else if (itemNameID == "SpeedPotion")
		{
			Potion* tempPotion = new SpeedPotion();
			items.push_back(tempPotion);
		}
		else if (itemNameID == "StrengthPotion")
		{
			Potion* tempPotion = new StrengthPotion();
			items.push_back(tempPotion);
		}
		else if (itemNameID == "Sword")
		{
			Sword* tempSword = new Sword(rarity);
			items.push_back(tempSword);
		}
		else if (itemNameID == "Axe")
		{
			Axe* tempAxe = new Axe(rarity);
			items.push_back(tempAxe);
		}
		else if (itemNameID == "Knives")
		{
			Knives* tempKnives = new Knives(rarity);
			items.push_back(tempKnives);

		}


		show(); 
		gold -= price; 

	}

}






void Inventory::sellItem(string itemNameID, float price)
{
	

	bool itemFound = false;
	for (iter = items.begin(); iter != items.end(); iter++)
	{
		if ((**iter).name == itemNameID)
		{
			//gold += (*iter).price; **** If want price inside Medkit ***** 
			gold += price;
			items.erase(iter);
			itemFound = true;
			break;
		}
	}

	if (itemFound == false)
	{
		cout << "Stop trying to break my test program by selling items that you don't have." << endl;
	}
}

Item* Inventory::FindItem(string itemNameID)
{
	bool itemFound = false;
	Item* tempItem;
	for (iter = items.begin(); iter != items.end(); ++iter)
	{
		cout << itemNameID;
		cout << (**iter).name;

		if ((**iter).name == itemNameID)
		{
			tempItem = &(**iter);
			cout << "Found Item";
			return tempItem;
			break;
		}


	}
	cout << "Didn't find item";
	return NULL;
}

Item* Inventory::GetItem(string itemNameID)
{
	bool itemFound = false;
	Item* tempItem;
	for (iter = items.begin(); iter != items.end(); iter++)
	{
		if ((**iter).name == itemNameID)
		{
			tempItem = &**iter;
			items.erase(iter);
			return tempItem;
			break;
		}
	}
	return NULL;
}

void Inventory::AddRandomItem()
{
	// generate a random number
	// pick a random reward based on that number 

	srand(time(0));
	int randomNumber = rand() % 100 + 1;

	if (randomNumber < 70) //70% chance on getting a potion. 
	{
		cout << "Making Potion";

		//mana and health are most likely  15 
		if ((randomNumber>0) && (randomNumber <= 15))
			addItem("HealthPotion");
		if ((randomNumber>15) && (randomNumber <= 30))
			addItem("ManaPotion");
		//then strength, speed, bonus armour and defence all equally likely (10%)
		if ((randomNumber>30) && (randomNumber <= 40))
			addItem("DefencePotion");
		if ((randomNumber>40) && (randomNumber <= 50))
			addItem("SpeedPotion");
		if ((randomNumber>50) && (randomNumber <= 60))
			addItem("BonusArmourPotion");
		if ((randomNumber>60) && (randomNumber <= 70))
			addItem("StrengthPotion");
	}
	else if (randomNumber > 70) //30% chance on getting a weapon. 
	{
		// 10% each. No need to generate extra random number as already between 70-100. 
		if ((randomNumber>70) && (randomNumber <= 80))
			addItem("Sword");
		if ((randomNumber>80) && (randomNumber <= 90))
			addItem("Axe");
		if ((randomNumber>90) && (randomNumber <= 100))
			addItem("Knives");
	}

}


void Inventory::UseItem(string itemNameID, Character* character)
{
	cout << "in UseItem()";

	Item* foundItem = FindItem(itemNameID);
	if (foundItem == NULL)
	{
		cout << "Item not found in inventory.";
		return;
	}

	foundItem->Use(character);
	removeItem(itemNameID);
}

void Inventory::show()
{
	cout << "Inventory (Size:" << getSize() << ") :" << endl;
	for (iter = items.begin(); iter != items.end(); ++iter)
	{
		cout << (**iter).name << endl;
	}
	cout << endl << "Gold: " << gold << endl << endl;
}

int Inventory::getSize()
{
	return items.size();
}

int Inventory::getCount(string id)
{
	int count = 0;
	for (iter = items.begin(); iter != items.end(); iter++)
	{
		if ((**iter).name == id)
		{
			count++;
		}
	}
	return count;
}
