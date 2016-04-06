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
	if (status != 0)
	{
		cout << endl << "QUEST ALREADY FINISHED" << endl;
		return;
	}
	else
	{
		if (enemy->characterName == ID)
		{
			cout << endl << "STATUS: QUEST COMPLETE" << endl;
			status = 1;
		}
		else
		{
			cout << endl << "STATUS: INCORRECT TARGET" << endl;
		}
	}
}

void Quest::Completion()
{
	if (status == 0)
	{
		cout << endl << "Quest is not complete! You are a cheat!" << endl;
	}
	if (status == 1)
	{
		cout << endl << "Dead Man: High Pay" << endl;
	}
	if (status == 2)
	{
		cout << endl << "Live Man: Low Pay" << endl;
	}
}

void Quest::DisplayQuest()
{
	cout << endl << name << ": " << description << endl;
}