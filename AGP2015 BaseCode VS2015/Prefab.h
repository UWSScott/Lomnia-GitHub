#pragma once
#ifndef PREFAB_H
#define PREFAB_H
#include "Gameobject.h"

class Prefab : public Gameobject
{
public:
	Prefab() {};
	Prefab(GLuint s_shaderProgram, char modelName, char * textureName, glm::vec3 s_position, glm::vec3 s_scale);
	virtual void draw(glm::mat4 object);

protected:
	GLuint meshObject;
};
#endif