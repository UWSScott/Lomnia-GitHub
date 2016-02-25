#include "Gameobject.h"

#define DEG_TO_RAD 0.017453293




void Gameobject::setShader(GLuint s_shaderProgram)
{ 
	shaderProgram = s_shaderProgram;
}

void Gameobject::draw(glm::mat4 object)
{
	glm::mat4 modelview(1.0);

	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	object = glm::translate(object, glm::vec3(position.x, position.y, position.z));
	object = glm::scale(object, glm::vec3(scale.x, scale.y, scale.z));
	object = glm::rotate(object, float(rotation*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
}

/*	Loads Texture, creating a quick instance of a class to use its function. */
void Gameobject::LoadTexture()
{
	FileLoader* imageLoader = new FileLoader;
	texture = imageLoader->loadBitmap("Lava");
	delete imageLoader;
}

/*	Sets inital material for the object. This is the default material. */
void Gameobject::SetMaterial()
{
	material =
	{
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
		{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
		{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
		2.0f  // shininess
	};

}
