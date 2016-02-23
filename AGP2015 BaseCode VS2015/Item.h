#pragma once
#ifndef ITEM_H
#define ITEM_H

#include <string>

using namespace std;
class Character;

class Item
{
public:
	int cost = 1;
	string name = "DEFAULT";

	Item() {};
	virtual void Apply(Character& attacker, Character& opponent); //TODO Move this to potion class... override with that items specific use ie health will heal etc.
};

#endif