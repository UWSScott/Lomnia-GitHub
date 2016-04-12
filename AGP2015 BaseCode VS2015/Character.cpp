#include "Character.h"
#include "Prefab.h"

struct MostDamageSort {
	MostDamageSort(Character& character) { this->character = character; }
	bool operator()(C_Attack attack_1, C_Attack attack_2) { return ((attack_1.damageCalc(character, *character.combatInstance->opponent) > attack_2.damageCalc(character, *character.combatInstance->opponent))); };
	Character character;
};

struct EfficentSort {
	EfficentSort(Character& character) { this->character = character; }
	bool operator()(C_Attack& attack_1, C_Attack& attack_2) { return (attack_1.GetBalanceValue() > attack_2.GetBalanceValue()); };
	Character character;
};

struct FastAttackSort {
	FastAttackSort(Character* character) { this->character = character; }
	bool operator()(C_Attack& attack_1, C_Attack& attack_2) { return (attack_1.AttackSpeed(character) > attack_2.AttackSpeed(character)); };
	Character* character;
};

Character::Character(string s_characterName, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram)
{
	//Collider = new Collisions();
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

	health = 5;
	scale = s_scale;
	position = s_position;

	FileLoader* fileLoader = new FileLoader;
	texture = fileLoader->loadBitmap(textureName);
	delete fileLoader;
}

Character::Character(string s_characterName, MD2Holder* modelInfo, TextureHolder* textureInfo, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram)
{
	Collider = new Collisions();
	detector = new Collisions();
	detector->radius = 1;

	characterName = s_characterName;
	shaderProgram = s_shaderprogram;
	material =
	{
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
		{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
		{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
		2.0f  // shininess
	};

	tmpModel = modelInfo->md2model;
	meshObject = modelInfo->model; //tmpModel.ReadMD2Model(modelName);
	md2VertCount = modelInfo->md2VertCount;
	texture = textureInfo->texture;

	health = 5;
	scale = s_scale;
	position = s_position;

	//FileLoader* fileLoader = new FileLoader;
	//texture = fileLoader->loadBitmap(textureName);
	//delete fileLoader;
}

Character::Character(string s_characterName, MD2Holder* modelInfo, TextureHolder* textureInfo, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram, int s_health, int s_strength, int s_speed, int s_defence)
{
	Collider = new Collisions();
	detector = new Collisions();
	detector->radius = 1;

	characterName = s_characterName;
	shaderProgram = s_shaderprogram;
	material =
	{
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
		{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
		{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
		2.0f  // shininess
	};

	tmpModel = modelInfo->md2model;
	meshObject = modelInfo->model;
	md2VertCount = modelInfo->md2VertCount;
	texture = textureInfo->texture;

	health = s_health;
	strength = s_strength;
	speed = s_speed;
	defence = s_defence;
	scale = s_scale;
	position = s_position;

}

glm::vec3 Character::MoveForward(glm::vec3 cam, GLfloat angle, GLfloat d)
{
	return glm::vec3(cam.x + d*std::sin(angle*DEG_TO_RAD), cam.y, cam.z - d*std::cos(angle*DEG_TO_RAD));
}

void Character::CheckQuestGoal(Character *character)
{
	//This should only apply to AI. If this passes the AI has defeated the player OR the ai was spared meaning we need to 
	//keep track of the current character as he will return at the end to help the player.
	//The player class overrides this, so any quest checking should be done inside that.
	cout << " CHARACTER CLASS " << endl;
	currentQuest->Conditions(character);
}

void Character::Dead()
{
	characterState = DEAD;
	status = STATE_DEATH;
}

void Character::LootEnemy(Character* character)
{
	coins += character->coins;
	xp += character->killXP;
	inventory->AddRandomItem(); // Loot random reward

								//All other transferable stats should go here.
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

void Character::EnterCombat(Character* opponent)
{
	status = STATE_COMBAT;
	combatInstance = new CombatInstance(this, opponent);
	canMove = false;
}

void Character::LeaveCombat()
{
	status = STATE_NORMAL;
	delete combatInstance;
	combatInstance = NULL;
	canMove = true;

	if (isDead())
	{
		status = STATE_DEATH;
	}
}

void Character::draw(glm::mat4 object)
{
	glUseProgram(shaderProgram);
	glCullFace(GL_FRONT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//glBindTexture(GL_TEXTURE_2D, texture);
	modelAt = MoveForward(position, rotation, 1.0f);
	object = glm::translate(object, MoveForward(position, rotation, 1.0f));
	object = glm::rotate(object, float((90.0f - rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	object = glm::scale(object, glm::vec3(scale.x*0.05, scale.y*0.05, scale.z*0.05));
	object = glm::rotate(object, float(90.0f*DEG_TO_RAD), glm::vec3(-1.0f, 0.0f, 0.0f));

	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	rt3d::drawMesh(meshObject, md2VertCount / 3, GL_TRIANGLES);
	//rt3d::drawMesh(meshObject, md2VertCount, GL_TRIANGLES);
	glCullFace(GL_BACK);

	if (weapon != NULL && weapon->getEquiped())
		weapon->draw(object, position, currentAnimation, rotation);
}

void Character::draw(glm::mat4 object, GLuint s_shaderUsed, int pass)
{
	glUseProgram(s_shaderUsed);
	glCullFace(GL_FRONT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);

	//glBindTexture(GL_TEXTURE_2D, texture);
	modelAt = MoveForward(position, rotation, 1.0f);
	object = glm::translate(object, MoveForward(position, rotation, 1.0f));
	object = glm::rotate(object, float((90.0f - rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	object = glm::scale(object, glm::vec3(scale.x*0.05, scale.y*0.05, scale.z*0.05));
	object = glm::rotate(object, float(90.0f*DEG_TO_RAD), glm::vec3(-1.0f, 0.0f, 0.0f));

	rt3d::setUniformMatrix4fv(s_shaderUsed, "modelview", glm::value_ptr(object));
	rt3d::drawMesh(meshObject, md2VertCount / 3, GL_TRIANGLES);

	if (weapon != NULL && weapon->getEquiped())
		weapon->draw(object, position, currentAnimation, rotation);

	glCullFace(GL_BACK);
}

void Character::GetAvailableAttacks(vector<C_Attack>& attackList)
{
	attackList.push_back(LightAttack());
	attackList.push_back(HeavyAttack());
	attackList.push_back(Poison());
	attackList.push_back(Stun());
}


void Character::CombatAttacks()
{
	if (isDead() == true || combatInstance == NULL)
		return;

	//All possible attacks!
	vector<C_Attack> possibleAttacks = vector<C_Attack>();
	GetAvailableAttacks(possibleAttacks);

	sort(possibleAttacks.begin(), possibleAttacks.end(), MostDamageSort(*this));
	C_Attack max_damage = possibleAttacks[0];
	sort(possibleAttacks.begin(), possibleAttacks.end(), FastAttackSort(this));
	C_Attack quickest_damage = possibleAttacks[0];
	sort(possibleAttacks.begin(), possibleAttacks.end(), EfficentSort(*this));
	C_Attack efficent_Attack = possibleAttacks[0];


	//Close to death, final gamble!
	//TODO check if character has health potion first!
	if (max_damage.GetPossibleDamage() >= combatInstance->opponent->health) { combatInstance->queuedAttacks.push_back(max_damage); return; }
	if (health <= rand() % 15 && max_damage.GetPossibleDamage() < combatInstance->opponent->health && inventory->getCount("Health_Potion") > 0) { combatInstance->queuedAttacks.push_back(ItemUse(inventory->GetItem("Health_Potion"))); return; }
	else if (health <= rand() % 15 && max_damage.GetPossibleDamage() < combatInstance->opponent->health) { combatInstance->queuedAttacks.push_back(Flee()); return; }

	int randomAttack = rand() % 3 + 1;
	switch (randomAttack)
	{
	case 1:
		combatInstance->queuedAttacks.push_back(max_damage); return;
		break;
	case 2:
		combatInstance->queuedAttacks.push_back(quickest_damage); return;
		break;
	case 3:
		combatInstance->queuedAttacks.push_back(efficent_Attack); return;
	default:
		int randomPossibleAttack = rand() % possibleAttacks.size();
		combatInstance->queuedAttacks.push_back(possibleAttacks[randomPossibleAttack - 1]); return;
		break;
	}

}

void Character::InitalStats(GLuint setShaderProgram) {}

void Character::CheckCollision(Gameobject* s_gameobject, string idType)
{


	cout << " idType : " << idType << endl;
	//(Character)s_gameobject->
	//cout << dynamic_cast <Prefab*>(s_gameobject)->position.x << endl;
	if (status == STATE_COMBAT || combatInstance != NULL)
		return;


	if (Character* d = dynamic_cast<Character*>(s_gameobject))
	{


		if (d->enemy)
		{
			EnterCombat(d);
			d->EnterCombat(this);
		}

		cout << " ****** ENEMY NAME IS " << d->characterName << " " << status << " " << canMove << " " << endl;

	}

	//if (objectID == "ENEMY")
	//{
	//	//EnterCombat()
	//}
}

void Character::Animate()
{
	if (!playAnimation)
		return;

	if (combatInstance != NULL)
		characterState = ATTACKING;
	if (isDead())
		characterState = DEAD;

	currentAnimation = characterState;



	tmpModel.Animate(currentAnimation, 0.1);
	rt3d::updateMesh(meshObject, RT3D_VERTEX, tmpModel.getAnimVerts(), tmpModel.getVertDataSize());
}

void Character::Update()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (combatInstance != NULL)
	{
		//cout << "HRERE" << endl;
		combatInstance->Update();
	}

	//characterState = IDLE;
	/*if (!isDead())
	{
		currentAnimation = 0;
		if (keys[SDL_SCANCODE_W])
		{
			characterState = WALKING;
			position = MoveForward(position, rotation, 0.1f);
		}
		else {
			//currentAnim = 0;
		}
		if (keys[SDL_SCANCODE_S])
		{
			characterState = WALKING;
			position = MoveForward(position, rotation, -0.1f);
		}

		if (keys[SDL_SCANCODE_A]) rotation -= 2.0f;
		if (keys[SDL_SCANCODE_D]) rotation += 2.0f;
		if (keys[SDL_SCANCODE_M]) characterState = ATTACKING;
	}*/

	Animate();
	//if (combatInstance == NULL)
	//	cout << " FSAFAS HIHIHIH 2222 ";
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

void Character::MoveToPlayer(Character* character)
{
	glm::vec3 MoveEye;
	MoveEye.x = character->position.x - this->position.x;
	MoveEye.z = character->position.z - this->position.z;

	if (MoveEye.x != 0 || MoveEye.z != 0 && inCombat == false)
		characterState = WALKING;
	else if (inCombat == false)
		characterState == IDLE;

	this->position.x += MoveEye.x*0.01f;
	this->position.z += MoveEye.z*0.01f;
}



//bool Character::getHighestDamage(C_Attack attack_1, C_Attack attack_2) { return true; }// (attack_1.damageCalc(*this, *combatInstance->opponent) > attack_2.damageCalc(*this, *combatInstance->opponent)); };
//bool Character::getFastestAttack(C_Attack& attack_1, C_Attack& attack_2) { return (attack_1.AttackSpeed(this) > attack_2.AttackSpeed(this)); };
//bool Character::getBalancedAttack(C_Attack& attack_1, C_Attack& attack_2) { return (attack_1.GetBalanceValue() > attack_2.GetBalanceValue()); };

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