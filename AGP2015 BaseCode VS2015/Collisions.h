#pragma once
#include "rt3d.h"
class Collisions
{
public:
	Collisions();
	GLfloat radius;
	GLfloat x;
	GLfloat z;

	GLfloat getRadius();
	GLfloat getX();
	GLfloat getZ();

	Collisions CollisionCircles(GLfloat x, GLfloat z, GLfloat radius);
	bool checkCollision(Collisions circle, Collisions circle2);

	~Collisions();
};

