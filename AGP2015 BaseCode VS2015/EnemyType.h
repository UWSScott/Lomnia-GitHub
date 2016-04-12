
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

