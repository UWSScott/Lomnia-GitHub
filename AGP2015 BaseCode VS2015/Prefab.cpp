#include "Prefab.h"

Prefab::Prefab(GLuint s_shaderProgram, char modelName, char * textureName, glm::vec3 s_position, glm::vec3 s_scale)
{
	collisionName = "STATIC_PREFAB";
	shaderProgram = s_shaderProgram;
	position = s_position;
	scale = s_scale;

	FileLoader* fileLoader = new FileLoader;
	texture = fileLoader->loadBitmap(textureName);
	delete fileLoader;
	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj(modelName +".obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
}

void Prefab::draw(glm::mat4 object)
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	object = glm::translate(object, position);
	object = glm::rotate(object, float((rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
}