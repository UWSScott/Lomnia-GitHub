#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H
#include "Gameobject.h"

#define IDLE 0
#define WALKING 1
#define ATTACKING 2
#define DEAD 16

//#include <iostream>
//#include <string>
#include <list>
#include <random>
#include <algorithm> 
#include "Weapon.h"
#include "Attack.h"
#include "CombatInstance.h"

class CombatInstance;

class Character : public Gameobject
{
private:
	float peviousTime = 0;


public:
	int DoIt(float a, char b, char c) { cout << "TMyClass::DoIt" << endl; return 1; };

	string characterName = "";
	int health = 10;
	int max_Health = 10;
	int manaPool;
	int max_Mana = 10;
	int strength = 5;
	int max_Strength = 10;
	int defence = 5;
	int max_Defence = 5;
	int speed = 5;
	int max_Speed = 10;
	Weapon* weapon = new Weapon();
	//Armor
	//Inventory List

	float resistance_Fire = 0;
	float resistance_Water = 0;
	float resistance_Air = 0;
	int coins = 0;
	int xp = 0;
	int killXP = 0;
	bool canDie = true;
	bool inCombat = false;
	int currentAnimation = 0;
	int characterState = 0;
	float refreshTime = 0;
	CombatInstance* combatInstance;
	virtual void CombatAttacks();
	virtual void BlockAttack();

	Character() {};
	Character(string s_characterName, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram);
	virtual void Update();
	virtual void Animate();
	virtual void InitalStats(GLuint setShaderProgram);
	virtual void draw(glm::mat4 object);
	virtual void GetAvailableAttacks(vector<C_Attack>& attackList);
	virtual  glm::vec3 getModelEye();
	virtual int getRotation();

	float ResSelect(int resType);
	virtual bool isDead();
	virtual void LootEnemy(Character* character);
	virtual void CheckQuestGoal(Character* character);
	virtual void Dead();

protected:

	//bool getHighestDamage(C_Attack attack_1, C_Attack attack_2);
	//bool getFastestAttack(C_Attack& attack_1, C_Attack& attack_2);
	//bool getBalancedAttack(C_Attack& attack_1, C_Attack& attack_2);
	glm::vec3 modelAt;
	glm::vec3 modelUp;

	md2model tmpModel;
	GLuint md2VertCount = 0;

	virtual glm::vec3 MoveForward(glm::vec3 cam, GLfloat angle, GLfloat d);

};


//bool (Character::*ptgetHighestDamage)(C_Attack, C_Attack) const = NULL;
//bool (Character::*getFastestAttack)(C_Attack, C_Attack) const = NULL;
//bool (Character::*getBalancedAttack)(C_Attack, C_Attack) const = NULL;

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

	float ResSelect(int resType);
	float physRes;
	float fireRes;
	float waterRes;
	float windRes;

	Character() {};
	Character(string s_name, int s_health, int s_mana, int s_def, int s_str, float s_physRes, float s_fireRes, float s_waterRes, float s_windRes, bool s_player);
	
	void BeingAttacked(C_Attack s_attack);
	void Update(float time);
	void Damage(int damageValue);
};*/

#endif