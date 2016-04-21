#include "ItemUse.h"
#include "Item.h"
#include "Character.h"

ItemUse::ItemUse(Item* s_itemObject)
{
	itemObject = s_itemObject;
}

void ItemUse::Attack(Character& attacker, Character& opponent)
{
	itemObject->Use(&attacker);
}