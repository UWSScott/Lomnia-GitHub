#include "PlayableCharacter.h"


PlayableCharacter::PlayableCharacter(string setName, int setHealth, int setStrength)
{
	rotation = 0;
	characterName = setName;
	health = setHealth;
	strength = setStrength;
	weapon = new Weapon("Scott's Saber", "Partical_sword.MD2", "hobgoblin2.bmp", 0, 5, 5, "SWORD", 1, shaderProgram);
	weapon->setEquiped(true);
	cout << " name: " << weapon->collisionName;
}

void PlayableCharacter::Input()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (inCombat == false)
	{
		characterState = IDLE;
		if (keys[SDL_SCANCODE_A]) { characterState = IDLE;  rotation -= 1.0f; }
		if (keys[SDL_SCANCODE_D]) { characterState = IDLE;  rotation += 1.0f; }
		if (keys[SDL_SCANCODE_W]) { characterState = WALKING;  position = MoveForward(position, rotation, 0.1f); }
		if (keys[SDL_SCANCODE_S]) { characterState = WALKING;  position = MoveForward(position, rotation, -0.1f); }
	}
}

void PlayableCharacter::Update()
{
	Input();
}



void PlayableCharacter::draw(glm::mat4 object)
{
	Animate();
	glUseProgram(shaderProgram);
	glCullFace(GL_FRONT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Animation
	tmpModel.Animate(currentAnimation, 0.1);
	rt3d::updateMesh(meshObject, RT3D_VERTEX, tmpModel.getAnimVerts(), tmpModel.getVertDataSize());

	//glBindTexture(GL_TEXTURE_2D, texture);
	modelAt = MoveForward(position, rotation, 1.0f);
	object = glm::translate(object, MoveForward(position, rotation, 1.0f));
	object = glm::rotate(object, float((90.0f - rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	object = glm::scale(object, glm::vec3(scale.x*0.05, scale.y*0.05, scale.z*0.05));
	object = glm::rotate(object, float(90.0f*DEG_TO_RAD), glm::vec3(-1.0f, 0.0f, 0.0f));

	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	rt3d::drawMesh(meshObject, md2VertCount, GL_TRIANGLES);
	glCullFace(GL_BACK);

	if (weapon->getEquiped())
		weapon->draw(object, position, currentAnimation, rotation);
}

