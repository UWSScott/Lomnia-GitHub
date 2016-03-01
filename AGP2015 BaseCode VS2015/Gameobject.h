#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#define DEG_TO_RAD 0.017453293
using namespace std;

#include "FileLoader.h"
#include "Collisions.h"
//#include "rt3d.h"
//#include "rt3dObjLoader.h"
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <stack>
//#include "md2model.h"
#include <SDL_ttf.h>

class Gameobject
{
public:
	glm::vec3 position = glm::vec3(1,1,1);
	glm::vec3 scale = glm::vec3(1,1,1);
	float rotation = 0;
	string collisionName = "DEFAULT";
	Collisions* Collider; 
	rt3d::materialStruct material;
	GLuint meshIndexCount;
	GLuint meshObject;
	GLuint shaderProgram;
	GLuint texture;

	Gameobject() {};
	virtual void setShader(GLuint s_shaderProgram);
	virtual void draw(glm::mat4 object);
	virtual void LoadTexture();
	virtual void SetMaterial();
};

#endif