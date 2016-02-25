#ifndef WEAPON_H
#define WEAPON_H

#include "Gameobject.h"
using namespace std;

class Weapon : public Gameobject
{

public:
	Weapon() {};
	Weapon(string s_objectName, char *modelName, char *textureName, int s_cost, float s_damage, float s_speed, string s_type, int s_rarity, GLuint s_shaderprogram);
	~Weapon() {};
	virtual void InitalStats(GLuint s_shaderprogram);
	void draw(glm::mat4 object, glm::vec3 playerPosition, int currentAnimation, int playerRotation);
	bool getEquiped() { return equiped; }
	void setEquiped(bool setting) { equiped = setting; }
protected:
	bool equiped = false;
	int cost = 0;
	float damage = 0;
	float speed = 0;
	string type = "DEFAULT";
	int rarity = 0;
	md2model tmpModel;
	GLuint md2VertCount = 0;
	GLuint shaderProgram;
	HSAMPLE *samples = NULL;
};
#endif