#include "Collisions.h"


Collisions::Collisions()
{
}
GLfloat radius;
GLfloat x;
GLfloat z;

GLfloat Collisions::getRadius() {

	return radius;
}
GLfloat Collisions::getX() {
	return x;
}
GLfloat Collisions::getZ() {
	return z;
}

Collisions Collisions::CollisionCircles(GLfloat ix, GLfloat iz, GLfloat r) {

	radius = r;
	x = ix;
	z = iz;

	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	const int NPOINTS = 25;
	const float TWOPI = 2 * 3.1415927;
	const float STEP = TWOPI / NPOINTS;
	for (float angle = 0; angle < TWOPI; angle += STEP)
		glVertex2f(x + radius*cos(angle), z + radius*sin(angle));
	glEnd();
	return Collisions();

}


bool Collisions::checkCollision(Collisions circle, Collisions circle2) 
{
	GLfloat radius = circle.getRadius();
	GLfloat x = circle.getX();
	GLfloat z = circle.getZ();

	GLfloat radius2 = circle2.getRadius();
	GLfloat x2 = circle2.getX();
	GLfloat z2 = circle2.getZ();

	int minDist = radius + radius2;
	int distance = sqrt(((x - x2) * (x - x2)) + ((z - z2) *(z - z2)));
	if (minDist >= distance) {
		return true;
	}
	else if ((minDist < distance))
	{
		return false;
	}

}





Collisions::~Collisions()
{
}
