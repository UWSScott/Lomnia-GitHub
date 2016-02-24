#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H
#include "Gameobject.h"

//#include <iostream>
//#include <string>
//using namespace std;
//#include <list>
//#include "Attack.h"

class Character : public Gameobject
{
private:
	float peviousTime = 0;
public:
	string characterName = "";
	int health = 10;
	int max_Health = 10;
	int manaPool;
	int max_Mana = 10;
	int strength = 5;
	int max_Strength = 10;
	int defence = 5;
	int max_Defence = 5;
	//Weapon
	//Armor
	//Inventory List
	float resistance_Fire = 0;
	float resistance_Water = 0;
	float resistance_Air = 0;
	int coins = 0;
	int xp = 0;
	bool canDie = true;

	Character() {};
	virtual void Update();
	virtual void InitalStats(GLuint setShaderProgram);
	virtual void draw(glm::mat4 object);
	virtual  glm::vec3 getModelEye();
	virtual int getRotation();

protected:
	glm::vec3 modelEye;
	glm::vec3 modelAt;
	glm::vec3 modelUp;
	int currentAnimation = 0;
	md2model tmpModel;
	GLuint md2VertCount = 0;

	virtual glm::vec3 MoveForward(glm::vec3 cam, GLfloat angle, GLfloat d);
	virtual bool isDead();
};

/*class Character : public Gameobject
{
private:
	float peviousTime = 0;
public:
	int health = 10;
	int manaPool;
	float refreshTime;
	int strength;
	int speed;
	int defence;
	bool player; //Only for prototype
	bool inCombat; //Only for prototype
	string name = "DEFAULT";
	list<C_Attack> queuedAttacks = list<C_Attack>();
	C_Attack opponentAttack;
	float ResSelect(int resType);
	float physRes;
	float fireRes;
	float waterRes;
	float windRes;

	Character() {};
	Character(string s_name, int s_health, int s_mana, int s_def, int s_str, float s_physRes, float s_fireRes, float s_waterRes, float s_windRes, bool s_player);
	void Attack(Character& enemyCharacter);
	void BeingAttacked(C_Attack s_attack);
	void Update(float time);
	void Damage(int damageValue);
};*/

#endif