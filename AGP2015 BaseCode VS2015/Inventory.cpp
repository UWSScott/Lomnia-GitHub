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
		itemInventory.push_back(tempPotion);
	} 
	else if (itemNameID == "ManaPotion")
	{
		ManaPotion tempPotion;
		itemInventory.push_back(tempPotion);
	}
	else if (itemNameID == "Sword")
	{
		Sword tempSword;
		itemInventory.push_back(tempSword);
	}
	else if (itemNameID == "Axe")
	{
		Axe tempAxe;
		itemInventory.push_back(tempAxe);
	}
	else if (itemNameID == "Knives")
	{
		Knives tempKnives;
		itemInventory.push_back(tempKnives);
	}

}

void Inventory::removeItem(string itemNameID)
{
	bool itemFound = false;

	for (iter = itemInventory.begin(); iter != itemInventory.end(); iter++)
	{
		if ((*iter).name == itemNameID)
		{
			itemInventory.erase(iter);
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
			itemInventory.push_back(tempPotion);
		}
		else if (itemNameID == "ManaPotion")
		{
			ManaPotion tempPotion;
			itemInventory.push_back(tempPotion);
		}
		else if (itemNameID == "Sword")
		{
			Sword tempSword;
			itemInventory.push_back(tempSword);
		}
		else if (itemNameID == "Axe")
		{
			Axe tempAxe;
			itemInventory.push_back(tempAxe);
		}
		else if (itemNameID == "Knives")
		{
			Knives tempKnives; //could check cost here instead after making object? seems inefficient
			itemInventory.push_back(tempKnives);
		}
		gold -= price;
	}
	
}

void Inventory::sellItem(string itemNameID, float price)
{
	bool itemFound =false; 

	for (iter = itemInventory.begin(); iter != itemInventory.end(); iter++)
	{
		if ((*iter).name==itemNameID)
		{
			//gold += (*iter).price; **** If want price inside Medkit *****
			gold += price;
			itemInventory.erase(iter);
			itemFound = true;
			break;
			
		}
	}

	if (itemFound == false)
	{
		cout << "Stop trying to break my test program by selling items that you don't have." << endl;
	}

}

void Inventory::show()
{
	cout << "Inventory (Size:" << getSize() << ") :" << endl;
	for (iter = itemInventory.begin(); iter != itemInventory.end(); ++iter)
	{
		cout << (*iter).name << endl;
	}
	cout << endl << "Gold: " << gold << endl << endl;
}

int Inventory::getSize()
{
	return itemInventory.size(); 
}

