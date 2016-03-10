#include "PlayableCharacter.h"


PlayableCharacter::PlayableCharacter(string s_characterName, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram)
{
	characterName = s_characterName;
	shaderProgram = s_shaderprogram;
	material =
	{
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
		{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
		{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
		2.0f  // shininess
	};
	meshObject = tmpModel.ReadMD2Model(modelName);
	md2VertCount = tmpModel.getVertDataSize()/3;

	weapon = new Weapon("Scott's Saber", "Models/Partical_sword.MD2", "hobgoblin2.bmp", 0, 5, 5, "SWORD", 1, shaderProgram);
	weapon->setEquiped(true);

	rotation = 0;
	scale = s_scale;
	position = s_position;

	FileLoader* fileLoader = new FileLoader;
	texture = fileLoader->loadBitmap(textureName);
	delete fileLoader;
}

/*PlayableCharacter::PlayableCharacter(string setName, int setHealth, int setStrength)
{
	rotation = 0;
	characterName = setName;
	health = setHealth;
	strength = setStrength;

	cout << " name: " << weapon->collisionName;
}*/

void PlayableCharacter::Dead()
{
	characterState = DEAD;
	//We need to add here what happens when the player dies. Ie reduce money and respawn himat the hub etc.
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
		if (keys[SDL_SCANCODE_X]) { characterState = ATTACKING; }
	} else if(inCombat == true && combatInstance != NULL){
		CombatAttacks();
	}
}

void PlayableCharacter::Update()
{
	Input();
}



//void PlayableCharacter::draw(glm::mat4 object)
//{
//	Animate();
//	glUseProgram(shaderProgram);
//	glCullFace(GL_FRONT);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture);
//
//	//Animation
//	tmpModel.Animate(currentAnimation, 0.1);
//	rt3d::updateMesh(meshObject, RT3D_VERTEX, tmpModel.getAnimVerts(), tmpModel.getVertDataSize());
//
//	//glBindTexture(GL_TEXTURE_2D, texture);
//	modelAt = MoveForward(position, rotation, 1.0f);
//	object = glm::translate(object, MoveForward(position, rotation, 1.0f));
//	object = glm::rotate(object, float((90.0f - rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
//	object = glm::scale(object, glm::vec3(scale.x*0.05, scale.y*0.05, scale.z*0.05));
//	object = glm::rotate(object, float(90.0f*DEG_TO_RAD), glm::vec3(-1.0f, 0.0f, 0.0f));
//
//	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
//	rt3d::drawMesh(meshObject, md2VertCount/3, GL_TRIANGLES);
//	glCullFace(GL_BACK);
//
//	if (weapon->getEquiped())
//		weapon->draw(object, position, currentAnimation, rotation);
//}

void PlayableCharacter::CheckQuestGoal(Character *character)
{
	//Ian do quest checking etc here - Scott.
}

void PlayableCharacter::CombatAttacks()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_1]) combatInstance->queuedAttacks.push_back(LightAttack());
	if (keys[SDL_SCANCODE_2]) combatInstance->queuedAttacks.push_back(HeavyAttack());
	if (keys[SDL_SCANCODE_3]) combatInstance->queuedAttacks.push_back(Poison());
	if (keys[SDL_SCANCODE_4]) combatInstance->queuedAttacks.push_back(Stun());
	if (keys[SDL_SCANCODE_5]) combatInstance->queuedAttacks.push_back(ItemUse(Item()));
	if (keys[SDL_SCANCODE_6]) combatInstance->queuedAttacks.push_back(Flee());

	/*if (combatInstance->incomingAttack.attackCompleted == false)
	{
		if (combatInstance->incomingAttack.blockingTime >= 0)
		{
			combatInstance->incomingAttack.blockingTime -= combatInstance->peviousTime;
			if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT]) // shouldn't these be 1, 2, 3 and 4 according to the output??
			{
				switch (combatInstance->incomingAttack.blockingButton)
				{
				case 1:
					if (keys[SDL_SCANCODE_UP])
						combatInstance->incomingAttack.BlockedAttack(*this, *this);
					else
						combatInstance->incomingAttack.FailedBlockedAttack(*this, *this);
					break;
				case 2:
					if (keys[SDL_SCANCODE_DOWN])
						combatInstance->incomingAttack.BlockedAttack(*this, *this);
					else
						combatInstance->incomingAttack.FailedBlockedAttack(*this, *this);
					break;
				case 3:
					if (keys[SDL_SCANCODE_LEFT])
						combatInstance->incomingAttack.BlockedAttack(*this, *this);
					else
						combatInstance->incomingAttack.FailedBlockedAttack(*this, *this);
					break;
				case 4:
					if (keys[SDL_SCANCODE_RIGHT])
						combatInstance->incomingAttack.BlockedAttack(*this, *this);
					else
						combatInstance->incomingAttack.FailedBlockedAttack(*this, *this);
					break;
				default:
					break;
				}
			}
		}
		else {
			combatInstance->incomingAttack.FailedBlockedAttack(*this, *this);
		}
	}*/
}

void PlayableCharacter::BlockAttack()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (combatInstance->incomingAttack.attackCompleted == false)
	{
		if (combatInstance->incomingAttack.blockingTime >= 0)
		{
			combatInstance->incomingAttack.blockingTime -= combatInstance->peviousTime;
			if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT]) // shouldn't these be 1, 2, 3 and 4 according to the output??
			{
				switch (combatInstance->incomingAttack.blockingButton)
				{
				case 1:
					if (keys[SDL_SCANCODE_UP])
						combatInstance->incomingAttack.BlockedAttack(*this, *this);
					else
						combatInstance->incomingAttack.FailedBlockedAttack(*this, *this);
					break;
				case 2:
					if (keys[SDL_SCANCODE_DOWN])
						combatInstance->incomingAttack.BlockedAttack(*this, *this);
					else
						combatInstance->incomingAttack.FailedBlockedAttack(*this, *this);
					break;
				case 3:
					if (keys[SDL_SCANCODE_LEFT])
						combatInstance->incomingAttack.BlockedAttack(*this, *this);
					else
						combatInstance->incomingAttack.FailedBlockedAttack(*this, *this);
					break;
				case 4:
					if (keys[SDL_SCANCODE_RIGHT])
						combatInstance->incomingAttack.BlockedAttack(*this, *this);
					else
						combatInstance->incomingAttack.FailedBlockedAttack(*this, *this);
					break;
				default:
					break;
				}
			}
		}
		else {
			combatInstance->incomingAttack.FailedBlockedAttack(*this, *this);
		}
	}
}
