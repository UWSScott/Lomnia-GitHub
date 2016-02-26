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
	} else if(inCombat == true && combatInstance != NULL){

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

void PlayableCharacter::CombatAttacks()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_1]) combatInstance->queuedAttacks.push_back(LightAttack());
	if (keys[SDL_SCANCODE_2]) combatInstance->queuedAttacks.push_back(HeavyAttack());
	if (keys[SDL_SCANCODE_3]) combatInstance->queuedAttacks.push_back(Poison());
	if (keys[SDL_SCANCODE_4]) combatInstance->queuedAttacks.push_back(Stun());
	//if (keys[SDL_SCANCODE_5]) combatInstance->queuedAttacks.push_back(ItemUse(Item()));
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
