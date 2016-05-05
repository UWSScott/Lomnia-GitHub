#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "Gameobject.h"

#define BRONZE_WEAPON_PRICE 100
#define SILVER_WEAPON_PRICE 500
#define GOLD_WEAPON_PRICE 1000

using namespace std;
class Character;

class Item : public Gameobject
{
public:
	char * name = "DEFAULT";
	string item_ID;
	float price;
	int rarityOrPotency;

	Item() {};
//	virtual void Apply(Character& attacker, Character& opponent); // Scotts for poison
	void virtual Use(Character* character) = 0;
	//void virtual equip(Character* character) = 0; 

};

#endif