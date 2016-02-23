#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
using namespace std;

#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include "md2model.h"
#include <SDL_ttf.h>

class Gameobject
{
public:
	glm::vec3 position;
	GLuint shaderProgram;

	/*float refreshTime = 5;
	int manaCost = 1s;
	int minDamage = 1;
	int maxDamage = 1;
	float bleedTime = 1;
	float blockingTime = 3;
	int blockingButton = 1;
	int blockingStatus = 0;
	string attackText = "DEFAULT";
	bool attackCompleted = false;
	int resType; // 0 = normal, 1 = fire, 2 = water, 3 = wind*/
    //Character combat_Opponent; //Only for prototype - a better system is needed, but for quickness this works atm.

	Gameobject() {};
	//virtual void Attack(Character& attacker, Character &opponent, int block);
	virtual void draw(glm::mat4 object, GLuint s_shaderUsed);
};

#endif