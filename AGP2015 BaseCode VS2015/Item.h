#pragma once
#ifndef ITEM_H
#define ITEM_H

#include <string>
#include "Gameobject.h"

using namespace std;
class Character;

class Item : public Gameobject
{
public:
	string name = "DEFAULT";
	string item_ID;
	float price = 1;

	Item() {};
	virtual void Apply(Character& attacker, Character& opponent); //TODO Move this to potion class... override with that items specific use ie health will heal etc.
	virtual void Use(Character* character) {};
};

#endif