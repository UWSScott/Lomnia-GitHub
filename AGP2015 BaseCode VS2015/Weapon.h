#ifndef WEAPON_H
#define WEAPON_H

#include "Item.h"
using namespace std;
#define RARITY_BRONZE 1
#define RARITY_SILVER 2
#define RARITY_GOLD 3

class Weapon : public Item
{

public:
	Weapon() {};
	Weapon(string s_objectName, char *modelName, char *textureName, int s_cost, float s_damage, float s_speed, string s_type, int s_rarity, GLuint s_shaderprogram);
	~Weapon() {};
	virtual void InitalStats(GLuint s_shaderprogram);
	void draw(glm::mat4 object, glm::vec3 playerPosition, int currentAnimation, int playerRotation);
	void draw(glm::mat4 object, glm::vec3 playerPosition, int currentAnimation, int playerRotation, GLuint s_shaderUsed, GLuint depthTexture, int pass);
	bool getEquiped() { return equiped; }
	void setEquiped(bool setting) { equiped = setting; }
	void Use(Character* character) {};
	int cost = 0;
	float attackPower = 0;
	float speed = 0;
protected:
	bool equiped = false;

	string type = "DEFAULT";
	int rarity = 0;
	md2model tmpModel;
	GLuint md2VertCount = 0;
	GLuint shaderProgram;
	HSAMPLE *samples = NULL;
};
#endif