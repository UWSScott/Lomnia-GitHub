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
	GetEdge(testPosition);

	//cout << " Position:  X: " << position.x << " y: " << position.y << " z: " << position.z << " scale: " << scale.x << " y: " << scale.y << " z: " << scale.z << endl;
}

//void MazePrefab::draw(glm::mat4 object, GLuint s_shaderProgram, int pass)
//{
//	glUseProgram(s_shaderProgram);
//	glCullFace(GL_FRONT);
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, texture);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
//
//	object = glm::translate(object, testPosition);
//	object = glm::scale(object, scale);
//	rt3d::setUniformMatrix4fv(s_shaderProgram, "modelview", glm::value_ptr(object));
//	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
//}

void MazePrefab::draw(glm::mat4 object, GLuint s_shaderUsed, int pass)
{
	glUseProgram(s_shaderUsed);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);

	if (pass == 0)
		glCullFace(GL_BACK);
	else
		glCullFace(GL_FRONT);

	object = glm::translate(object, glm::vec3(testPosition.x, testPosition.y, testPosition.z));
	object = glm::scale(object, glm::vec3(scale.x, scale.y, scale.z));
	object = glm::rotate(object, float(rotation*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	rt3d::setUniformMatrix4fv(s_shaderUsed, "modelview", glm::value_ptr(object));
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
}