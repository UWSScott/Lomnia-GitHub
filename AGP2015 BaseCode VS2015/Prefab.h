#pragma once
#ifndef PREFAB_H
#define PREFAB_H
#include "Gameobject.h"

class Prefab : public Gameobject
{
public:
	Prefab() {};
	Prefab(GLuint s_shaderProgram, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position, float s_rotation = 0);
	virtual void draw(glm::mat4 object);
	virtual void draw(glm::mat4 object, GLuint s_shaderUsed, int pass);
	GLuint getObject() { return meshObject; }
protected:
	GLuint meshObject;
};
#endif