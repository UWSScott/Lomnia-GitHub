
#ifndef MINION_H
#define MINION_H
#include "Character.h"
class Minion : public Character
{
private:
public:
	Minion() {};
	Minion(string s_characterName, MD2Holder* modelInfo, TextureHolder* textureInfo, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram)
	{
		enemy = true;
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

		health = 5;
		speed = 4;
		strength = 4;
		defence = 4;
	};

	~Minion() {};
protected:

};

#endif

#ifndef BRUISER_H
#define BRUISER_H
#include "Character.h"

class Bruiser : public Character
{
private:
public:
	Bruiser() {};
	Bruiser(string s_characterName, MD2Holder* modelInfo, TextureHolder* textureInfo, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram)
	{
		enemy = true;
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

		health = 5;
		speed = 4;
		strength = 4;
		defence = 4;
	};

	~Bruiser() {};
protected:
};
#endif


#ifndef OVERLORD_H
#define OVERLORD_H
#include "Character.h"
class Overlord : public Character
{
private:
public:
	Overlord() {};
	Overlord(string s_characterName, MD2Holder* modelInfo, TextureHolder* textureInfo, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram)
	{
		enemy = true;
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

		health = 5;
		speed = 4;
		strength = 4;
		defence = 4;
	};

	~Overlord() {};
protected:
};
#endif

#ifndef GODUS_H
#define GODUS_H
#include "Character.h"
class GODUS : public Character
{
private:
public:
	GODUS() {};
	GODUS(string s_characterName, MD2Holder* modelInfo, TextureHolder* textureInfo, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram)
	{
		enemy = true;
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

		health = 5;
		speed = 4;
		strength = 4;
		defence = 4;
	};

	~GODUS() {};
protected:
};
#endif

#ifndef QUESTGIVER_H
#define QUESTGIVER_H
#include "Character.h"
class QuestGiver : public Character
{
private:
public:

	vector<Quest*> available_quests = vector<Quest*>();

	QuestGiver() {};
	QuestGiver(MD2Holder* modelInfo, TextureHolder* textureInfo, glm::vec3 s_scale, glm::vec3 s_position, GLuint s_shaderprogram)
	{
		available_quests.push_back(new Quest("Kill the Mutated Ripper", "Find the Blue Ripper and eliminate it", "Blue_Ripper", "MINION", "Models/ripper.MD2", "Models/Textures/Bronze_Skin.bmp", 100, 0));
		available_quests.push_back(new Quest("Destroy Pogo-Bunny", "Locate and kill the upgraded Pogo-Bunny", "Silver_Pogo", "BRUISER", "Models/pogo_buny.MD2", "Models/Textures/Silver_Skin.bmp", 250, 0));
		available_quests.push_back(new Quest("Slay the Gold Dragon", "Find the Golden Dragon and eliminate it!", "Gold_Dragon", "OVERLORD", "Models/dragon.MD2", "Models/Textures/Gold_Skin.bmp", 500, 0));
		available_quests.push_back(new Quest("Free Lomnia", "Defeat the god to earn your freedom!", "Tintania", "GODUS", "Models/faerie.MD2", "Models/Textures/God_World.bmp", 0, 0));

		characterName = "Blade, The Giver of Quests!";
		enemy = false;
		canDie = false;
		canDraw = true;
		Collider = new Collisions();
		detector = new Collisions();
		detector->radius = 1;

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
		scale = glm::vec3(1);
		position = s_position;
		health = 5;
		speed = 4;
		strength = 4;
		defence = 0;
	};

	Quest* GetQuest(int completedQuests)
	{
		return available_quests[completedQuests];
	}

	~QuestGiver() {};
protected:
};
#endif
