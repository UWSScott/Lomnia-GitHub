#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H
#include "Prefab.h"

class Terrain : public Prefab
{
public:
	Terrain() {};
	Terrain(GLuint s_shaderProgram, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position);
	virtual void draw(glm::mat4 object);
};
#endif