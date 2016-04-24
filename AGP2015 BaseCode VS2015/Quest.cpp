#include "Quest.h"
#include "Character.h"

//Quest* quest_1 = new Quest("Kill the Mutated Ripper", "Find the Blue Ripper and eliminate it", "Blue_Ripper", "MINION", "ripper", "Bronze_Skin", 100, 0);
//Quest* quest_2 = new Quest("Destroy Pogo-Bunny", "Locate and kill the upgraded Pogo-Bunny", "Silver_Pogo", "BRUISER", "pogo_buny", "Silver_Skin", 250, 0);
//Quest* quest_3 = new Quest("Slay the Gold Dragon", "Find the Golden Dragon and eliminate it!", "Gold_Dragon", "OVERLORD", "dragon", "Gold_Skin", 500, 0);
//Quest* quest_4 = new Quest("Free Lomnia", "Defeat the god to earn your freedom!", "Tintania", "GODUS", "faerie", "Chain_Link", 0, 0);

Quest* questList[4] = {};// quest_1, quest_2, quest_3, quest_4 };

Quest::Quest(string s_name, string s_description, string s_ID, string s_type, char* s_targetModel, char* s_targetTexture, int s_reward, int s_status)
{
	name = s_name;
	description = s_description;
	ID = s_ID;
	type = s_type;
	targetModel = s_targetModel;
	targetTexture = s_targetTexture;
	reward = s_reward;
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
	if (status == 1)
	{
		cout << endl << "Dead Man: High Pay" << endl;

	}
	if (status == 2)
	{
		cout << endl << "Live Man: Low Pay" << endl;
	}
	else
	{
		cout << endl << "Quest is not complete!" << endl;
	}
}

void Quest::DisplayQuest()
{
	cout << endl << name << ": " << description << endl;
}

void Quest::QuestSet(Character* character, int questNumber)
{
	character->currentQuest = questList[questNumber];
}

