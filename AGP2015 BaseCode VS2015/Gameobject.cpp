#include "Gameobject.h"
#define DEG_TO_RAD 0.017453293
//Hi Ian write this!!!dsadsfsdfdsjhfdsjka
//fdsafjdksafds
//dasafjkadsfjkdsal
//fjkdasfjkd

void Gameobject::draw(glm::mat4 object, GLuint s_shaderUsed)
{
	glm::mat4 modelview(1.0);

	glUseProgram(s_shaderUsed);
	if (hatchingShader)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, hatchShaders[0]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, hatchShaders[1]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, hatchShaders[2]);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, hatchShaders[3]);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, hatchShaders[4]);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, hatchShaders[5]);
	}
	else {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	}

	object = glm::translate(object, glm::vec3(xPosition, yPosition, zPosition));
	object = glm::scale(object, glm::vec3(scaleX, scaleY, scaleZ));
	object = glm::rotate(object, float(rotation*DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
	rt3d::setUniformMatrix4fv(s_shaderUsed, "modelview", glm::value_ptr(object));
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
}