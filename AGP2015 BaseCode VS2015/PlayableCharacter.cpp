#include "PlayableCharacter.h"


PlayableCharacter::PlayableCharacter(string setName, int setHealth, int setStrength)
{
	characterName = setName;
	health = setHealth;
	strength = setStrength;
}


void PlayableCharacter::draw(glm::mat4 object)
{
	glUseProgram(shaderProgram);
	glCullFace(GL_FRONT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Animation
	tmpModel.Animate(currentAnimation, 0.1);
	rt3d::updateMesh(meshObject, RT3D_VERTEX, tmpModel.getAnimVerts(), tmpModel.getVertDataSize());

	// drawing the player model

	//glBindTexture(GL_TEXTURE_2D, texture);
	modelAt = MoveForward(position, rotation, 1.0f);
	object = glm::translate(object, MoveForward(position, rotation, 1.0f));
	object = glm::rotate(object, float((90.0f - rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	object = glm::scale(object, glm::vec3(scale.x*0.05, scale.y*0.05, scale.z*0.05));
	object = glm::rotate(object, float(90.0f*DEG_TO_RAD), glm::vec3(-1.0f, 0.0f, 0.0f));

	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	rt3d::drawMesh(meshObject, md2VertCount, GL_TRIANGLES);
	glCullFace(GL_BACK);

	//if (weapon.getEquiped())
	//	weapon.draw(object, position, currentAnimation, rotation);
}
