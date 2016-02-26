#pragma once
#ifndef PLAYABLECHARACTER_H
#define PLAYABLECHARACTER_H
#include "Character.h"

class PlayableCharacter : public Character
{
private:
public:
	PlayableCharacter() {};
	~PlayableCharacter() {};
	PlayableCharacter(string setName, int setHealth, int setStrength);
	virtual void draw(glm::mat4 object);
	virtual void Update();
	virtual void CombatAttacks();
	virtual void BlockAttack();
	void Input();
	//weapon = Weapon("Scott's Saber", "Partical_sword.MD2", "hobgoblin2.bmp", 0, 5, 5, "SWORD", 1, shaderProgram);
	/*string characterName = "";
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

	PlayableCharacter() {};
	virtual void Update();
	virtual void InitalStats(GLuint setShaderProgram);
	virtual void draw(glm::mat4 object);
	virtual  glm::vec3 getModelEye();
	virtual int getRotation();*/

protected:
};
#endif