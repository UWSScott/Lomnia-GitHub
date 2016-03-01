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

using namespace std;


class Inventory
{
private:
	vector<string> inventory;
	vector<Item> itemInventory;
	vector<Item>::iterator iter;
	float gold; 
	

public: 
	void show(); 
	void sellItem(string itemNameID, float price); 
	void buyItem(string itemNameID, float price); 
	void addItem(string itemNameID); 
	void removeItem(string itemNameID);
	int getSize();
	Inventory();

	
};