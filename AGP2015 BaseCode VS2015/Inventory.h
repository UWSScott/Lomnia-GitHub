#ifndef INVENTORY_H
#define INVENTORY_H

#include <iostream> 
#include <vector>
#include <ctime> 
#include <cstdlib> 
#include <string>
#include "Item.h"
#include "Medkit.h"
#include "ManaPotion.h"
#include "Sword.h"
#include "Axe.h"
#include "Knives.h"
//#include "Character.h"

using namespace std;

class Character;

class Inventory
{
private:
	//vector<string> inventory

public: 
	void show(); 
	void sellItem(string itemNameID, float price); 
	void buyItem(string itemNameID, float price); 
	void addItem(string itemNameID);
	Item* FindItem(string itemNameID);
	Item* GetItem(string itemNameID);
	void UseItem(string itemNameID, Character* character);
	void removeItem(string itemNameID);
	int getSize();
	int getCount(string id);
	Inventory();

	vector<Item> items;
	vector<Item>::iterator iter;
	float gold;
	
};

#endif