#ifndef CAMERA_H
#define CAMERA_H

#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include "Gameobject.h"
#include "FileLoader.h"

class Camera : public Gameobject
{
public:
	Camera(){ InitalStats(); };
	~Camera(){};
	void InitalStats();
	void draw(glm::mat4 &object, glm::vec3 modelEye);
	void CombatCinematic(glm::mat4 &object, glm::vec3 modelEye);
	void update(glm::vec3 modelEye, float playerRotation);
	void Sound(int soundFile);
	glm::vec3 MoveForward(glm::vec3 cam, GLfloat angle, GLfloat d);
	float rotation;

	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
private:
	int camera_Type = 1; //1-Normal(locked), 2-Cinematic Camera, 3-Free camera
	HSAMPLE *songs = NULL;
};

#endif