#include "Character.h"
#include <random>
#include <algorithm>  

Character::Character(string s_characterName, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram)
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
	md2VertCount = tmpModel.getVertDataSize();

	scale = s_scale;
	position = s_position;

	FileLoader* fileLoader = new FileLoader;
	texture = fileLoader->loadBitmap(textureName);
	delete fileLoader;
}

glm::vec3 Character::MoveForward(glm::vec3 cam, GLfloat angle, GLfloat d)
{
	return glm::vec3(cam.x + d*std::sin(angle*DEG_TO_RAD), cam.y, cam.z - d*std::cos(angle*DEG_TO_RAD));
}

bool Character::isDead()
{
	return (health <= 0 && canDie);
}

// returns the current rotation of the model
int Character::getRotation()
{
	return rotation;
}

/* returns position of player (Location of player) */
glm::vec3 Character::getModelEye()
{
	return position;
}

void Character::draw(glm::mat4 object)
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

	if (weapon->getEquiped())
		weapon->draw(object, position, currentAnimation, rotation);
}

void Character::CombatAttacks()
{
	if (isDead() == true)
		return;

	vector<C_Attack> possibleAttacks = vector<C_Attack>();
	possibleAttacks.push_back(LightAttack());
	possibleAttacks.push_back(HeavyAttack());
	possibleAttacks.push_back(Poison());
	possibleAttacks.push_back(Stun());
	possibleAttacks.push_back(LightAttack());
	sort(possibleAttacks.begin(), possibleAttacks.end(), getHighestDamage);
	C_Attack max_damage = possibleAttacks[0];
	sort(possibleAttacks.begin(), possibleAttacks.end(), getFastestAttack);
	C_Attack quickest_damage = possibleAttacks[0];

	float killableChance = 0;


	//Close to death, final gamble!
	//TODO check if character has health potion first!
	if (max_damage.GetPossibleDamage() >= combatInstance->opponent->health){ combatInstance->queuedAttacks.push_back(max_damage); return; }
	if (health <= rand() % 15 && max_damage.GetPossibleDamage() < combatInstance->opponent->health) { combatInstance->queuedAttacks.push_back(ItemUse(Item())); return; }
	else if (health <= rand() % 15 && max_damage.GetPossibleDamage() < combatInstance->opponent->health) { combatInstance->queuedAttacks.push_back(Flee()); return; }


	//if (keys[SDL_SCANCODE_1]) combatInstance->queuedAttacks.push_back(LightAttack());
	//if (keys[SDL_SCANCODE_2]) combatInstance->queuedAttacks.push_back(HeavyAttack());
	//if (keys[SDL_SCANCODE_3]) combatInstance->queuedAttacks.push_back(Poison());
	//if (keys[SDL_SCANCODE_4]) combatInstance->queuedAttacks.push_back(Stun());
	//if (keys[SDL_SCANCODE_5]) combatInstance->queuedAttacks.push_back(ItemUse(Item()));
	//if (keys[SDL_SCANCODE_6]) combatInstance->queuedAttacks.push_back(Flee());
}

void Character::InitalStats(GLuint setShaderProgram)
{
	//LoadFromFile();





}

void Character::Animate()
{
	currentAnimation = characterState;
	//switch (characterState)
	//{
	//case IDLE:
	//	currentAnimation = 0;
	//	break;
	//case WALKING:
	//	currentAnimation = 1;
	//	break;
	//case ATTACKING:
	//	currentAnimation = 
	//default:
	//	currentAnimation = 0;
	//	break;
	//}
}

void Character::Update()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (!isDead())
	{
		currentAnimation = 0;
		if (keys[SDL_SCANCODE_W])
		{
			currentAnimation = 1;
			position = MoveForward(position, rotation, 0.1f);
		}
		else {
			//currentAnim = 0;
		}
		if (keys[SDL_SCANCODE_S])
		{
			currentAnimation = 1;
			position = MoveForward(position, rotation, -0.1f);
		}

		if (keys[SDL_SCANCODE_A]) rotation -= 5.0f;
		if (keys[SDL_SCANCODE_D]) rotation += 5.0f;
	}
}




float Character::ResSelect(int resType)
{
	if (resType == 0) { return 0; }
	if (resType == 1) { return resistance_Fire; }
	if (resType == 2) { return resistance_Water; }
	if (resType == 3) { return resistance_Air; }
	return 0;
}

void Character::BlockAttack()
{
	if (combatInstance->incomingAttack.attackCompleted == false)
	{
		int randomNumber = rand() % 15 + 1;
		if (speed >= randomNumber)
		{
			//TODO use character stats to get an accurate value for blocking based on character stats.
			float timeBlocked = randomNumber = rand() % (int)(combatInstance->incomingAttack.blockingTime);
			combatInstance->incomingAttack.blockingTime = timeBlocked;
			combatInstance->incomingAttack.BlockedAttack(*this, *this);
		}
		else {
			combatInstance->incomingAttack.FailedBlockedAttack(*this, *this);
		}
	}
}

/*
#include "rt3d.h"
#include "Stun.h"
#include "Poison.h"
#include "ItemUse.h"
#include "Flee.h"
#include "HeavyAttack.h"
#include "LightAttack.h"

void LimitVariable(float &variable, float minValue, float maxValue)
{
	if (variable > maxValue)
		variable = maxValue;
	else if (variable < minValue)
		variable = minValue;
}

void LimitVariable(int &variable, int minValue, int maxValue)
{
	if (variable > maxValue)
		variable = maxValue;
	else if (variable < minValue)
		variable = minValue;
}

Character::Character(string s_name, int s_health, int s_mana, int s_def,int s_str, float s_physRes, float s_fireRes, float s_waterRes, float s_windRes, bool s_player)
{
	name = s_name;
	health = s_health;
	manaPool = s_mana;
	defence = s_def;
	strength = s_str;
	physRes = s_physRes;
	fireRes = s_fireRes;
	waterRes = s_waterRes;
	windRes = s_windRes;
	player = s_player; 

}




void Character::Update(float time)
{
	float timeDifference = (time - peviousTime);
	refreshTime -= timeDifference;
	peviousTime = time;
	LimitVariable(refreshTime, 0, 999);

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (inCombat)
	{
		if (player)
		{
			if (opponentAttack.blockingTime >= 0 && opponentAttack.attackCompleted == false)
			{
				opponentAttack.blockingTime -= timeDifference;
				if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT]) // shouldn't these be 1, 2, 3 and 4 according to the output??
				{
					switch (opponentAttack.blockingButton)
					{
					case 1:
						if (keys[SDL_SCANCODE_UP])
							opponentAttack.BlockedAttack(*this, *this);
						else
							opponentAttack.FailedBlockedAttack(*this, *this);
						break;
					case 2:
						if (keys[SDL_SCANCODE_DOWN])
							opponentAttack.BlockedAttack(*this, *this);
						else
							opponentAttack.FailedBlockedAttack(*this, *this);
						break;
					case 3:
						if (keys[SDL_SCANCODE_LEFT])
							opponentAttack.BlockedAttack(*this, *this);
						else
							opponentAttack.FailedBlockedAttack(*this, *this);
						break;
					case 4:
						if (keys[SDL_SCANCODE_RIGHT])
							opponentAttack.BlockedAttack(*this, *this);
						else
							opponentAttack.FailedBlockedAttack(*this, *this);
						break;
					default:
						break;
					}
				}
			} else {
				if (opponentAttack.blockingTime <= 0 && opponentAttack.attackCompleted == false && inCombat == true && player == true)
				{
					opponentAttack.FailedBlockedAttack(*this, *this);
				}
			}
		} else {
			if (opponentAttack.blockingTime >= 0 && opponentAttack.attackCompleted == false)
			{
				if(rand() % 100 >= 70)
					opponentAttack.BlockedAttack(*this, *this);
				else
					opponentAttack.FailedBlockedAttack(*this, *this);
			}
		}
	}

	
}
*/