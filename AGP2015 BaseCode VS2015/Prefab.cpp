#include "Prefab.h"

Prefab::Prefab(GLuint s_shaderProgram, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position, float s_rotation, glm::vec3 s_maxVec, glm::vec3 s_minVec)
{
	Collider = new Collisions(s_minVec, s_maxVec);
	collisionName = "STATIC_PREFAB";
	shaderProgram = s_shaderProgram;
	position = s_position;
	scale = s_scale;
	rotation = s_rotation;
	FileLoader* fileLoader = new FileLoader;
	texture = fileLoader->loadBitmap(textureName);
	delete fileLoader;
}

Prefab::Prefab(GLuint s_shaderProgram, OBJHolder* modelInfo, TextureHolder* textureInfo, glm::vec3 s_scale, glm::vec3 s_position, float s_rotation, glm::vec3 s_maxVec, glm::vec3 s_minVec)
{
	Collider = new Collisions(s_minVec, s_maxVec);
	collisionName = "STATIC_PREFAB";
	shaderProgram = s_shaderProgram;
	position = s_position;
	scale = s_scale;
	rotation = s_rotation;
	meshObject = modelInfo->model;
	meshIndexCount = modelInfo->meshCount;
	texture = textureInfo->texture;

	//FileLoader* fileLoader = new FileLoader;
	//texture = fileLoader->loadBitmap(textureName);
	//delete fileLoader;
}


Prefab::Prefab(string ItemID, GLuint s_shaderProgram, OBJHolder* modelInfo, TextureHolder* textureInfo, glm::vec3 s_scale, glm::vec3 s_position, float s_rotation, glm::vec3 s_maxVec, glm::vec3 s_minVec)
{
	objectName = ItemID;
	Collider = new Collisions(s_minVec, s_maxVec);
	collisionName = "STATIC_PREFAB";
	shaderProgram = s_shaderProgram;
	position = s_position;
	scale = s_scale;
	rotation = s_rotation;
	meshObject = modelInfo->model;
	meshIndexCount = modelInfo->meshCount;
	texture = textureInfo->texture;
	GetEdge(position);

	//FileLoader* fileLoader = new FileLoader;
	//texture = fileLoader->loadBitmap(textureName);
	//delete fileLoader;
}

void Prefab::draw(glm::mat4 object)
{
	if (!canDraw)
		return;

	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	object = glm::translate(object, position);
	object = glm::scale(object, scale);
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
}

void Prefab::draw(glm::mat4 object, GLuint s_shaderUsed, int pass)
{
	if (!canDraw)
		return;

	glUseProgram(s_shaderUsed);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);

	if (pass == 0)
		glCullFace(GL_BACK);
	else
		glCullFace(GL_FRONT);

	object = glm::translate(object, glm::vec3(position.x, position.y, position.z));
	object = glm::scale(object, glm::vec3(scale.x, scale.y, scale.z));
	object = glm::rotate(object, float(rotation*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	rt3d::setUniformMatrix4fv(s_shaderUsed, "modelview", glm::value_ptr(object));
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
}

void Prefab::GetEdge(glm::vec3 currentPosition)
{
	glm::vec3 topRight;
	glm::vec3 bottomLeft;

	topRight.x = (currentPosition.x + (scale.x / 2));
	topRight.y = (currentPosition.y + (scale.y / 2));
	topRight.z = (currentPosition.z + (scale.z / 2));

	bottomLeft.x = (currentPosition.x - (scale.x / 2));
	bottomLeft.y = (currentPosition.y - (scale.y / 2));
	bottomLeft.z = (currentPosition.z - (scale.z / 2));
	Collider = new Collisions(glm::vec3(topRight), glm::vec3(bottomLeft));
}