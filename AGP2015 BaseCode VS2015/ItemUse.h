#pragma once
#ifndef ITEMUSE_H
#define ITEMUSE_H

//class Item;
#include "Item.h"
#include "Attack.h"
class ItemUse : public C_Attack
{
private:
	Item* itemObject;
public:
	ItemUse() { attackText = "ItemUse";  CreateBlockingButton(); };
	ItemUse(Item* s_itemObject);
	void Attack(Character& attacker, Character& opponent);
};


#endif

