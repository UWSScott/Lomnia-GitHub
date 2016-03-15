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

#define STATE_LOADING 0
#define STATE_NORMAL 1
#define STATE_COMBAT 2



class Gameobject
{
public:
	string objectName = "";
	glm::vec3 position = glm::vec3(1,1,1);
	glm::vec3 scale = glm::vec3(1,1,1);
	float rotation = 0;
	string collisionName = "DEFAULT";
	Collisions* Collider = new Collisions();
	rt3d::materialStruct material;
	GLuint meshIndexCount;
	GLuint meshObject;
	GLuint shaderProgram;
	GLuint texture;
	GLuint depthMapTexture;
	glm::vec3 maxVec;
	glm::vec3 minVec;


	Gameobject() {};
	virtual void setShader(GLuint s_shaderProgram);
	virtual void draw(glm::mat4 object);
	virtual void draw(glm::mat4 object, GLuint s_shaderUsed, int pass);
	virtual void LoadTexture();
	virtual void SetMaterial();
	virtual void SetDepthMap(GLuint s_depthMap) { depthMapTexture = s_depthMap; };
};
#endif