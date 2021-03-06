#include "PlayableCharacter.h"
#include "Medkit.h"


PlayableCharacter::PlayableCharacter(string s_characterName, char *modelName, char *textureName, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram)
{
	characterName = s_characterName;
	shaderProgram = s_shaderprogram;
	status = STATE_NORMAL;

	material =
	{
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
		{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
		{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
		2.0f  // shininess
	};
	meshObject = tmpModel.ReadMD2Model(modelName);
	md2VertCount = tmpModel.getVertDataSize() / 3;

	weapon = new Weapon("Scott's Saber", "Models/Partical_sword.MD2", "hobgoblin2.bmp", 0, 5, 5, "SWORD", 1, shaderProgram);
	//weapon = new Sword(1); // starts with bronze sword. 
	weapon->setEquiped(true);

	canDie = true;
	rotation = 0;
	health = 100;
	manaPool = 100;
	scale = s_scale;
	position = s_position;
	Collider = new Collisions();
	currentQuest = new Quest("Kill Blade", "Blade is a nasty man!", "Blade", "OVERLORD", "Models/quigon.MD2", "Models/Textures/Bronze_Skin.bmp", 500, 0);

	//Potion* tempPotion = new Medkit(1);
	inventory->addItem("HealthPotion", POTION_POTENCY_MEDIUM);
	inventory->addItem("HealthPotion", POTION_POTENCY_MEDIUM);
	inventory->addItem("ManaPotion", POTION_POTENCY_MEDIUM);
	inventory->addItem("ManaPotion", POTION_POTENCY_MEDIUM);
	cout << " health potions count: " << inventory->getCount("HealthPotion");

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
	status = STATE_DEATH;
	cout << " GOT HERE ";
	//We need to add here what happens when the player dies. Ie reduce money and respawn himat the hub etc.
}

void PlayableCharacter::Input()
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (combatInstance == NULL && !isDead())
	{
		characterState = IDLE;
		if (canMove == false)
			return;

		if (keys[SDL_SCANCODE_A]) { characterState = IDLE;  rotation -= 1.0f; }
		if (keys[SDL_SCANCODE_D]) { characterState = IDLE;  rotation += 1.0f; }
		if (keys[SDL_SCANCODE_W])
		{
			if (rotation > 89 && rotation < 186)
			{
				if (position[0] + 0.1 < 105.6)
				{
					characterState = WALKING;  position = MoveForward(position, rotation, 0.1f);
				}
			}
			else if (rotation > 0 && rotation < 90)
			{
				if (position[2] + 0.1 > -50.6)
				{
					characterState = WALKING;  position = MoveForward(position, rotation, 0.1f);
				}
			}
			else if (rotation < 0 && rotation > -94)
			{
				if (position[0] + 0.1 >-34)
				{
					characterState = WALKING;  position = MoveForward(position, rotation, 0.1f);
				}
			}
			else if (rotation < -94 && rotation > -184)
			{
				if (position[2] + 0.1 <20)
				{
					characterState = WALKING;  position = MoveForward(position, rotation, 0.1f);
				}
			}
		else
		{
			characterState = WALKING;  position = MoveForward(position, rotation, 0.1f);
		}
		}

		if (keys[SDL_SCANCODE_S]) { characterState = WALKING;  position = MoveForward(position, rotation, -0.1f); }
		if (keys[SDL_SCANCODE_X]) { characterState = ATTACKING; }
	}
	else if (combatInstance != NULL) {
		combatInstance->Update();
		//BlockAttack();
		CombatAttacks();

	
	}
}

void PlayableCharacter::Update(float frameRate, Camera* camera)
{
	Input();
	Animate(frameRate);
	RegenMana();

	if (camera != NULL)
		camera->SetPlayerStatus(status, this);

	if (weapon != inventory->equippedWeapon)
		equipWeapon(); // check if new weapon has been equipped, if so change to it.


	
}



void PlayableCharacter::draw(glm::mat4 object)
{
	glUseProgram(shaderProgram);
	glCullFace(GL_FRONT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	modelAt = MoveForward(position, rotation, 1.0f);
	object = glm::translate(object, MoveForward(position, rotation, 1.0f));
	object = glm::rotate(object, float((90.0f - rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	object = glm::scale(object, glm::vec3(scale.x*0.05, scale.y*0.05, scale.z*0.05));
	object = glm::rotate(object, float(90.0f*DEG_TO_RAD), glm::vec3(-1.0f, 0.0f, 0.0f));

	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	rt3d::drawMesh(meshObject, md2VertCount / 3, GL_TRIANGLES);
	glCullFace(GL_BACK);

	if (weapon != NULL && weapon->getEquiped())
		weapon->draw(object, position, currentAnimation, rotation);
}

void PlayableCharacter::draw(glm::mat4 object, GLuint s_shaderUsed, int pass)
{
	//if (weapon != NULL && weapon->getEquiped())
		weapon->draw(object, position, currentAnimation, rotation, s_shaderUsed, depthMapTexture, pass);

	glUseProgram(s_shaderUsed);
	glCullFace(GL_FRONT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);

	modelAt = MoveForward(position, rotation, 1.0f);
	object = glm::translate(object, MoveForward(position, rotation, 1.0f));
	object = glm::rotate(object, float((90.0f - rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	object = glm::scale(object, glm::vec3(scale.x*0.05, scale.y*0.05, scale.z*0.05));
	object = glm::rotate(object, float(90.0f*DEG_TO_RAD), glm::vec3(-1.0f, 0.0f, 0.0f));

	rt3d::setUniformMatrix4fv(s_shaderUsed, "modelview", glm::value_ptr(object));
	rt3d::drawMesh(meshObject, md2VertCount, GL_TRIANGLES);
	glCullFace(GL_BACK);
}

//void PlayableCharacter::CheckQuestGoal(Character *character)
//{
//	//Ian do quest checking etc here - Scott.
//	cout << " PLAYER CLASS" << endl;
//}

void PlayableCharacter::CombatAttacks()
{
	if (isDead() == true || combatInstance == NULL)
		return;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_1]) combatInstance->Input(new LightAttack());
	if (keys[SDL_SCANCODE_2]) combatInstance->Input(new HeavyAttack());
	if (keys[SDL_SCANCODE_3]) combatInstance->Input(new Poison());
	if (keys[SDL_SCANCODE_4]) combatInstance->Input(new Stun());
	if (keys[SDL_SCANCODE_5]) combatInstance->Input(new Flee());

	if (keys[SDL_SCANCODE_H])
	{
		//cout << " got here for healthHealthPotion  " << inventory->getCount("HealthPotion") << endl;
		if (inventory->getCount("HealthPotion") > 0)
		{
			cout << " HealthPotion count : " << inventory->getCount("HealthPotion") << endl;
			combatInstance->Input(new ItemUse(inventory->FindItem("HealthPotion")));
			//cout << " Health Restored! " << endl;
		}
	}

	if (keys[SDL_SCANCODE_J])
	{
		if (inventory->getCount("ManaPotion") > 0)
		{
			cout << " Mana count : " << inventory->getCount("ManaPotion") << endl;
			combatInstance->Input(new ItemUse(inventory->FindItem("ManaPotion")));
		}
	}
}

void PlayableCharacter::equipWeapon()
{
	weapon = inventory->equipWeapon(); 
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
