#pragma once
#ifndef PLAYABLECHARACTER_H
#define PLAYABLECHARACTER_H
#include "Character.h"
#include "Camera.h"

class Camera;

class PlayableCharacter : public Character
{
private:
public:
	PlayableCharacter() {};
	~PlayableCharacter() {};
	PlayableCharacter(string s_characterName, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram);
	virtual void draw(glm::mat4 object);
	virtual void draw(glm::mat4 object, GLuint s_shaderUsed, int pass);
	virtual void Update(float frameRate, Camera* camera = NULL);
	virtual void CombatAttacks();
	virtual void BlockAttack();
	//virtual void CheckQuestGoal(Character *character);
	virtual void Dead();
	void Input();

	void equipWeapon();



protected:
};
#endif