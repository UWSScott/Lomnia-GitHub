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
#include "BonusArmourPotion.h"
#include "DefencePotion.h"
#include "SpeedPotion.h"
#include "StrengthPotion.h"
//#include "PlayableCharacter.h"
//#include "UI.h"

using namespace std;

class Character;
class PlayableCharacter; 



#define BRONZE_WEAPON_PRICE 100
#define SILVER_WEAPON_PRICE 500
#define GOLD_WEAPON_PRICE 1000
#define LIGHT_POTION_PRICE 12
#define MEDIUM_POTION_PRICE 20
#define STRONG_POTION_PRICE 30
#define POTION_POTENCY_LIGHT 4
#define POTION_POTENCY_MEDIUM 5
#define POTION_POTENCY_STRONG 6

class Inventory
{
private:
	//vector<string> inventory

public:
	void show();
	void sellItem(string itemNameID, int rarity);
	void buyItem(string itemNameID, int rarityOrPotency);
	void addItem(string itemNameID, int rarityOrPotency);
	Item* FindItem(string itemNameID);
	//Item* GetItem(string itemNameID);
	bool hasItem(string itemNameID);
	void AddRandomItem(); // To be called after item pick up / generate random quest reward
	void UseItem(string itemNameID, Character* character);
	void removeItem(string itemNameID);
	int getSize();
	int getCount(string id);
	//void equipWeapon(string itemNameID, Character* PlayableCharacter);
	Inventory();
	//UI * menuUI;
	Weapon* equippedWeapon; 

	

	Weapon* equipWeapon();

	vector<Item*> items;
	vector<Item*>::iterator iter;
	float gold;

};

#endif