#include "Inventory.h"
#include <stdlib.h>
#include <ctime>



Inventory::Inventory()
{
	gold = 1000.00f; 
	equippedWeapon = new Sword(1); 
}

void Inventory::addItem(string itemNameID, int rarityOrPotency)
{
	// find out what item to add, create item, push onto inventory
	if (itemNameID == "HealthPotion")
	{
		Potion* tempPotion = new Medkit(rarityOrPotency);
		items.push_back(tempPotion);
	}
	else if (itemNameID == "ManaPotion")
	{
		Potion* tempPotion = new ManaPotion(rarityOrPotency);
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
		Sword* tempSword = new Sword(rarityOrPotency);
		items.push_back(tempSword);
	}
	else if (itemNameID == "Axe")
	{
		Axe* tempAxe = new Axe(rarityOrPotency);
		items.push_back(tempAxe);
	}
	else if (itemNameID == "Knives")
	{
		Knives* tempKnives = new Knives(rarityOrPotency);
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

void Inventory::buyItem(string itemNameID, int rarityOrPotency)
{
	int playerChoice;
	int price; 

		switch (rarityOrPotency)
		{
		case 1:
			price = BRONZE_WEAPON_PRICE;
			break;
		case 2:
			price = SILVER_WEAPON_PRICE;
			break;
		case 3:
			price = GOLD_WEAPON_PRICE;
			break;
		case 4: 
			price = LIGHT_POTION_PRICE;
			break; 
		case 5:
			price = MEDIUM_POTION_PRICE; 
			break; 
		case 6: 
			price = STRONG_POTION_PRICE; 
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
			Potion* tempPotion = new Medkit(rarityOrPotency);
		
			items.push_back(tempPotion);
			show();
			
		}
		else if (itemNameID == "ManaPotion")
		{
			Potion* tempPotion = new ManaPotion(rarityOrPotency);
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
			Sword* tempSword = new Sword(rarityOrPotency);
			items.push_back(tempSword);
			equippedWeapon = tempSword; 
		}
		else if (itemNameID == "Axe")
		{
			Axe* tempAxe = new Axe(rarityOrPotency);
			items.push_back(tempAxe);
			equippedWeapon = tempAxe;
		}
		else if (itemNameID == "Knives")
		{
			Knives* tempKnives = new Knives(rarityOrPotency);
			items.push_back(tempKnives);
			equippedWeapon = tempKnives;

		}

		gold -= price; 
		show();
	}
}

bool Inventory::sellItem(string itemNameID, int rarity)
{
	bool itemFound = false;
	for (iter = items.begin(); iter != items.end(); iter++)
	{
		if (((**iter).name == itemNameID)&&((**iter).rarityOrPotency==rarity))
		{
			gold = gold + ((**iter).price)/2;

			items.erase(iter);
			itemFound = true;
			cout << "Sold item." << endl;
			return true;
		}
	}

	if (itemFound == false)
	{
		cout << "Stop trying to break my test program by selling items that you don't have." << endl;
		return false; 
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


void Inventory::AddRandomItem()
{
	// generate a random number
	// pick a random reward based on that number 
	// rand potency 

	// 70 % weak, 
	// 71-90 medium 
	// 90-100 strong 

	srand(time(0));
	int randomNumber = rand() % 101 + 1;
	gold += rand() % 51 + 25; // gold with item 
	int potencyOrRarity; 
	string item; 
	

		cout << "Making Potion";

		//mana and health are most likely  30 
		if ((randomNumber > 0) && (randomNumber <= 50))
			item = "HealthPotion"; 
			
		if ((randomNumber>50) && (randomNumber <= 100))
			item = "ManaPotion"; 

		randomNumber = rand() % 101 + 1;
		int potency;
		if (randomNumber < 70) //70% chance on getting a potion. 
		{
			potency = POTION_POTENCY_LIGHT;
		}
		if (randomNumber >= 70 && randomNumber<91) //70% chance on getting a potion. 
		{
			potency = POTION_POTENCY_MEDIUM;
		}
		else if (randomNumber > 90 && randomNumber < 101)
		{
			potency = POTION_POTENCY_STRONG;
		}
		else
			potency = POTION_POTENCY_LIGHT;

		addItem(item, potency); 

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

bool Inventory::hasItem(string itemNameID)
{
	bool itemFound = false;
	for (iter = items.begin(); iter != items.end(); ++iter)
	{
		if ((**iter).name == itemNameID)
		{
			return true;
		}
	}

	return false;
}

Weapon* Inventory::equipWeapon()
{
	return equippedWeapon; 
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
