#include "MazePrefab.h"

MazePrefab::MazePrefab(GLuint s_shaderProgram, GLuint s_meshObject, GLuint s_meshIndexCount, GLuint s_texture, glm::vec3 s_scale, glm::vec3 s_position)
{
	collisionName = "Maze Object";
	shaderProgram = s_shaderProgram;
	this->position = s_position;
	testPosition = s_position;
	scale = s_scale;
	texture = s_texture;
	meshIndexCount = s_meshIndexCount;
	meshObject = s_meshObject;

	//cout << " Position:  X: " << position.x << " y: " << position.y << " z: " << position.z << " scale: " << scale.x << " y: " << scale.y << " z: " << scale.z << endl;
}

void MazePrefab::draw(glm::mat4 object)
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//cout << " Position:  X: " << testPosition.x << " y: " << testPosition.y << " z: " << testPosition.z << " scale: " << scale.x << " y: " << scale.y <<" z: " << scale.z << endl;

	object = glm::translate(object, testPosition);
	object = glm::scale(object, scale);
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
}