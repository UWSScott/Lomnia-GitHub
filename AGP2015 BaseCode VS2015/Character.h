#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H
#include "Gameobject.h"

#define IDLE 0
#define WALKING 1
#define ATTACKING 2
#define HURT_1 3
#define HURT_2	4
#define HURT_3	5
#define DEAD 17



//#define MD2_STAND	0
//#define MD2_RUN		1
//#define MD2_ATTACK	2
//#define MD2_PAIN1	3
//#define MD2_PAIN2	4
//#define MD2_PAIN3	5
//#define MD2_JUMP	6
//#define MD2_FLIP	7
//#define MD2_SALUTE	8
//#define MD2_TAUNT	9
//#define MD2_WAVE	10
//#define MD2_POINT	11
//#define MD2_CRSTAND	12
//#define MD2_CRWALK	13
//#define MD2_CRATTACK	14
//#define MD2_CRPAIN	15
//#define MD2_CRDEATH	16
//#define MD2_DEATH1	17
//#define MD2_DEATH2	18
//#define MD2_DEATH3	19




//#include <iostream>
//#include <string>
#include <list>
#include <random>
#include <algorithm> 
#include "Quest.h"
#include "Weapon.h"
#include "Attack.h"
#include "CombatInstance.h"
#include "Inventory.h"

class CombatInstance;


class Character : public Gameobject
{
private:
	float peviousTime = 0;


public:
	//int DoIt(float a, char b, char c) { cout << "TMyClass::DoIt" << endl; return 1; };

	string characterName = "";
	string ID = "";
	int health = 100;
	int max_Health = 100;
	int manaPool = 10;
	int max_Mana = 10;
	int strength = 5;
	int max_Strength = 10;
	int defence = 5;
	int max_Defence = 5;
	int speed = 5;
	int max_Speed = 10;
	Quest* currentQuest = new Quest();
	Weapon* weapon = new Weapon();
	Inventory* inventory = new Inventory();
	//Armor
	//Inventory List
	
	float resistance_Fire = 0;
	float resistance_Water = 0;
	float resistance_Air = 0;
	int coins = 0;
	int level = 1;
	int xp = 0;
	int killXP = 0;
	bool canDie = true;
	bool inCombat = false;
	bool playAnimation = true;
	int currentAnimation = 0;
	int characterState = 0;
	int status = 0;
	float refreshTime = 0;
	CombatInstance* combatInstance = NULL;
	virtual void CombatAttacks();
	virtual void BlockAttack();

	Character() {};
	Character(string s_characterName, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram);
	Character(string s_characterName, MD2Holder* modelInfo, TextureHolder* textureInfo, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram);
	virtual void Update();
	virtual void Animate();
	virtual void Damage(float damageValue) { health -= damageValue; };
	virtual void InitalStats(GLuint setShaderProgram);
	virtual void draw(glm::mat4 object);
	virtual void draw(glm::mat4 object, GLuint s_shaderUsed, int pass);
	virtual void GetAvailableAttacks(vector<C_Attack>& attackList);
	virtual void CheckCollision(Gameobject* s_gameobject, string idType);
	virtual  glm::vec3 getModelEye();
	virtual int getRotation();
	virtual void EnterCombat(Character* opponent);
	virtual void LeaveCombat();
	//void Attack(Character& enemyCharacter);
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