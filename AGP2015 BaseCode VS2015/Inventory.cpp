#include "Inventory.h"

Inventory::Inventory()
{
	
	gold = 50.00f; 
}

void Inventory::addItem(string itemNameID)
{
	// find out what item to add, create item, push onto inventory
	if (itemNameID == "Medkit")
	{
		Medkit tempPotion(1);
		items.push_back(tempPotion);
	} 
	else if (itemNameID == "ManaPotion")
	{
		ManaPotion tempPotion;
		items.push_back(tempPotion);
	}
	else if (itemNameID == "Sword")
	{
		Sword tempSword;
		items.push_back(tempSword);
	}
	else if (itemNameID == "Axe")
	{
		Axe tempAxe;
		items.push_back(tempAxe);
	}
	else if (itemNameID == "Knives")
	{
		Knives tempKnives;
		items.push_back(tempKnives);
	}
}

void Inventory::removeItem(string itemNameID)
{
	bool itemFound = false;

	for (iter = items.begin(); iter != items.end(); iter++)
	{
		if ((*iter).name == itemNameID)
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

void Inventory::buyItem(string itemNameID, float price)
{
	int playerChoice; 

	if (price > gold)
	{
		cout << "Sorry, you don't have enough gold to buy a " << itemNameID << endl;
	}
	else
	{
		if (itemNameID == "Medkit")
		{
			cout << "What potency of Medkit would you like to buy? " << endl;
			cout << "1. Light" << endl << "2. Medium" << endl << "3. Heavy" << endl << "4. Holy"; 
			cin >> playerChoice; 
			Medkit tempPotion(playerChoice);
			items.push_back(tempPotion);
		}
		else if (itemNameID == "ManaPotion")
		{
			ManaPotion tempPotion;
			items.push_back(tempPotion);
		}
		else if (itemNameID == "Sword")
		{
			Sword tempSword;
			items.push_back(tempSword);
		}
		else if (itemNameID == "Axe")
		{
			Axe tempAxe;
			items.push_back(tempAxe);
		}
		else if (itemNameID == "Knives")
		{
			Knives tempKnives; //could check cost here instead after making object? seems inefficient
			items.push_back(tempKnives);
		}
		gold -= price;
	}
	
}

void Inventory::sellItem(string itemNameID, float price)
{
	bool itemFound = false; 
	for (iter = items.begin(); iter != items.end(); iter++)
	{
		if ((*iter).name==itemNameID)
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
	for (iter = items.begin(); iter != items.end(); iter++)
	{
		if ((*iter).name == itemNameID)
		{
			tempItem = &*iter;
			return tempItem;
			break;
		}
	}
	return NULL;
}

Item* Inventory::GetItem(string itemNameID)
{
	bool itemFound = false;
	Item* tempItem;
	for (iter = items.begin(); iter != items.end(); iter++)
	{
		if ((*iter).name == itemNameID)
		{
			tempItem = &*iter;
			items.erase(iter);
			return tempItem;
			break;
		}
	}
	return NULL;
}

void Inventory::UseItem(string itemNameID, Character* character)
{
	Item* foundItem = FindItem(itemNameID);
	if (foundItem == NULL)
		return;

	//TODO: Hayley needs to fix inventory.
	foundItem->Use(character);
	removeItem(itemNameID);
}

void Inventory::show()
{
	cout << "Inventory (Size:" << getSize() << ") :" << endl;
	for (iter = items.begin(); iter != items.end(); ++iter)
	{
		cout << (*iter).name << endl;
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
		if ((*iter).name == id)
		{
			count++;
		}
	}
	return count;
}
