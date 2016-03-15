#include "Quest.h"
//
////void init()
////{
//// killBill.name = "Kill Bill";
//// killBill.description = "Find Bill, kill him dead";
//// killBill.ID = "target02";
//// 
//// killWill.name = "Kill Will";
//// killWill.description = "Find Will, kill him dead";
//// killWill.ID = "target01";
////
//// QuestList[0] = killBill;
//// QuestList[1] = killWill;
////}
////
Quest::Quest(string s_name, string s_description, string s_ID, int s_status)
{
	name = s_name;
	description = s_description;
	ID = s_ID;
	status = s_status;
}

void Quest::Conditions(Character *enemy)
{
	//if (enemy->ID == ID)
	//{
	//	cout << endl << "Target confirmed";
	//	//status = 1 or 2
	//}
	//else
	//{
	//	cout << endl << "Not the right m8, m8" << endl;
	//	return;
	//}
}

void Quest::DisplayQuest()
{
	cout << endl << name << ": " << description << endl;
}