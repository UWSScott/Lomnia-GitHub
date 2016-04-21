#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "Gameobject.h"


using namespace std;
class Character;

class Item : public Gameobject
{
public:
	char * name = "DEFAULT";
	string item_ID;
	float price = 1;

	Item() {};
	virtual void Apply(Character& attacker, Character& opponent); // Scotts for poison
	void virtual Use(Character* character) = 0;
	//void virtual equip(Character* character) = 0; 

};

#endif