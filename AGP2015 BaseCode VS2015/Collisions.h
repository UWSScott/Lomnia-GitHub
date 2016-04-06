#pragma once
#include <glm/glm.hpp>
#include "rt3d.h"

class AABB
{
public:
	AABB() {};
	AABB(glm::vec3 vecMin, glm::vec3 vecMax) { this->vecMin = vecMin; this->vecMax = vecMax; }
	glm::vec3 vecMax;
	glm::vec3 vecMin;
};

class Collisions
{
public:
	Collisions();
	Collisions(glm::vec3 vecMin, glm::vec3 vecMax) { aabb = new AABB(vecMin, vecMax); }
	AABB* aabb;

	GLfloat radius = 0;
	GLfloat x = 0;
	GLfloat z = 0;

	GLfloat getRadius();
	GLfloat getX();
	GLfloat getZ();

	//AABB createCubes(glm::vec3 vecMax, glm::vec3 vecMin);
	Collisions CollisionCircles(GLfloat x, GLfloat z, GLfloat radius);

	bool checkCollision(Collisions* circle, Collisions* circle2);
	bool checkCollision(const AABB* tBox, const glm::vec3& vecPoint);

	~Collisions();
};

