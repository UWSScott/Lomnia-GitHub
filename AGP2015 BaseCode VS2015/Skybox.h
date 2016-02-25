#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H
#include <stack>

#include "Gameobject.h"
using namespace std;

class Skybox : public Gameobject
{

public:
	Skybox() {};
	~Skybox() {};
	void InitalStats(const char *skyboxmap[6]);
	void draw(glm::mat4 object);
	GLuint shaderProgram;
private:
	GLuint meshObject;
	GLuint textures[6];
	std::stack<glm::mat4> mvStack;

	GLuint meshIndexCount;
};
#endif