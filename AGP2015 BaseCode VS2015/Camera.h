#ifndef CAMERA_H
#define CAMERA_H

#define FIRST_PERSON 0
#define THIRD_PERSON 1
#define COMBAT_CINEMATIC 2
#define FREE_VIEW 3

#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <ctime>
#include "Gameobject.h"
#include "FileLoader.h"
#include "PlayableCharacter.h"

class PlayableCharacter;

class Camera : public Gameobject
{
public:
	Camera(){ InitalStats(); };
	~Camera(){};
	void InitalStats();
	void draw(glm::mat4 &object, glm::vec3 modelEye);
	void SwitchState(int state, PlayableCharacter* character);
	void TranslateTo(float &currentPosition, float &newPosition);
	void CombatCinematic(glm::mat4 &object, glm::vec3 modelEye);
	void CinematicValues(glm::vec3 characterPosition, float playerRotation);
	void update(glm::vec3 modelEye, float playerRotation);
	void Sound(int soundFile);
	void SetPlayerStatus(int status, PlayableCharacter* character);
	glm::vec3 MoveForward(glm::vec3 cam, GLfloat angle, GLfloat d);
	glm::vec3 MoveRight(glm::vec3 pos, GLfloat angle, GLfloat d);
	float rotation;

	glm::vec3 cinematicLookAt;

	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
private:
	std::clock_t start = 0;
	float y_mouse_Smoothing = 1.0f;
	float x_mouse_Smoothing = 2.0f;
	float camera_Pitch = 0.0f;
	double duration = 0;
	double timeDifference = 0;
	int camera_Type = 3; //First Person //1-Normal(locked), 2-Cinematic Camera, 3-Free camera
	float Cinematic_X = 0;
	float Cinematic_Y = 0;
	float Cinematic_Z = 0;
	float Cinematic_Timer = 0;
	float Cinematic_Movement = 0;
	HSAMPLE *songs = NULL;
};

#endif