#pragma once
#ifndef PREFAB_H
#define PREFAB_H
#include "Gameobject.h"

class Prefab : public Gameobject
{
public:
	Prefab() {};
	Prefab(GLuint s_shaderProgram, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position, float s_rotation = 0, glm::vec3 s_maxVec = glm::vec3(0), glm::vec3 s_minVec = glm::vec3(0));
	Prefab(GLuint s_shaderProgram, OBJHolder* modelInfo, TextureHolder* textureInfo, glm::vec3 s_scale, glm::vec3 s_position, float s_rotation, glm::vec3 s_maxVec, glm::vec3 s_minVec);
	Prefab(string ItemID, GLuint s_shaderProgram, OBJHolder* modelInfo, TextureHolder* textureInfo, glm::vec3 s_scale, glm::vec3 s_position, float s_rotation, glm::vec3 s_maxVec, glm::vec3 s_minVec);

	virtual void draw(glm::mat4 object);
	virtual void draw(glm::mat4 object, GLuint s_shaderUsed, int pass);
	GLuint getObject() { return meshObject; }
	void GetEdge(glm::vec3 currentPosition);
protected:
	GLuint meshObject;
};
#endif