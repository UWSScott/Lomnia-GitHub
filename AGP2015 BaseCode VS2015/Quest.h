#pragma once
#ifndef QUEST_H
#define QUEST_H


using namespace std;
#include <string>
#include<iostream>

class Character;

class Quest
{
public:
	Quest() {};
	Quest(char* s_name, char* s_description, string s_ID, string s_type, char* s_targetModel, char* s_targetTexture, int s_reward, int s_status);

	char* name;
	char* description;
	string ID;
	string type;
	char* targetModel;
	char* targetTexture;
	int reward;
	int status = 0; // 0 for incomplete, 1 for mercy, 2 for kill

	void Conditions(Character* enemy);
	void Completion();
	void DisplayQuest();
	void QuestSet(Character* character, int questNumber);
};

#endif