#include "Terrain.h"

Terrain::Terrain(GLuint s_shaderProgram, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position, float s_rotation)
{
	collisionName = "STATIC_PREFAB";
	shaderProgram = s_shaderProgram;
	position = s_position;
	scale = s_scale;
	rotation = s_rotation;

	FileLoader* fileLoader = new FileLoader;
	texture = fileLoader->loadBitmap(textureName);
	delete fileLoader;
	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj(modelName, verts, norms, tex_coords, indices);
	meshIndexCount = indices.size();
	meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), meshIndexCount, indices.data());

}

void Terrain::draw(glm::mat4 object)
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shaderProgram, "textureScaleModifier"), 80);
	object = glm::translate(object, position);
	object = glm::scale(object, scale);
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	glUniform1i(glGetUniformLocation(shaderProgram, "textureScaleModifier"), 1);
}

void Terrain::draw(glm::mat4 object, GLuint s_shaderUsed, int pass)
{
	glUseProgram(s_shaderUsed);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glUniform1i(glGetUniformLocation(s_shaderUsed, "textureScaleModifier"), 80);
	//if (pass == 1)
	//	glCullFace(GL_BACK);
	//else
	//	glCullFace(GL_FRONT);

	object = glm::translate(object, position);
	object = glm::scale(object, scale);
	object = glm::rotate(object, float(rotation*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	rt3d::setUniformMatrix4fv(s_shaderUsed, "modelview", glm::value_ptr(object));
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	glUniform1i(glGetUniformLocation(s_shaderUsed, "textureScaleModifier"), 1);
}