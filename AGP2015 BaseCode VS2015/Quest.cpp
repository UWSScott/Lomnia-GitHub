#include "Quest.h"
#include "Character.h"


Quest::Quest(string s_name, string s_description, string s_ID, int s_status)
{
	name = s_name;
	description = s_description;
	ID = s_ID;
	status = s_status;
}

void Quest::Conditions(Character* enemy)
{
	if (enemy->ID == ID)
	{
		cout << endl << "Target confirmed";
		//status = 1 or 2
	}
	else
	{
		cout << endl << "Not the right m8, m8" << endl;
		return;
	}
}

void Quest::DisplayQuest()
{
	cout << endl << name << ": " << description << endl;
}