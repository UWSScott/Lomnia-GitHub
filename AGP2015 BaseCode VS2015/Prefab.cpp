#include "Prefab.h"

Prefab::Prefab(GLuint s_shaderProgram, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position)
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
	rt3d::loadObj(modelName, verts, norms, tex_coords, indices);
	meshIndexCount = indices.size();
	meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), meshIndexCount, indices.data());

}

void Prefab::draw(glm::mat4 object)
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//object = glm::translate(object, position);
	//object = glm::rotate(object, float((rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	//object = glm::scale(object, glm::vec3(scale.x, scale.y, scale.z));
	//rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	//rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);

	//rt3d::setLightPos(shaderProgram, glm::value_ptr(tmp));
	
	object = glm::translate(object, glm::vec3(2.0, 4.0, 2.0));
	object = glm::scale(object,scale);
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
}