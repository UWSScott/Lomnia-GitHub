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
	Quest(string s_name, string s_description, string s_ID, string s_type, string s_targetModel, string s_targetTexture, int s_reward, int s_status) {};

	string name;
	string description;
	string ID;
	string type;
	string targetModel;
	string targetTexture;
	int reward;
	int status = 0; // 0 for incomplete, 1 for mercy, 2 for kill

	void Conditions(Character* enemy);
	void Completion();
	void DisplayQuest();
	void QuestSet(Character* character, int questNumber);
};

#endif