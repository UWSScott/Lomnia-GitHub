#pragma once
#include <SDL_ttf.h>
#include "rt3d.h"
#include "ResourceManager.h"
//#include "Gameobject.h"
#include <ctime>
#include "CombatInstance.h"
#include "Inventory.h"
#include "PlayableCharacter.h"







class UI
{
public:
	UI();

	ResourceManager* Resource_Managment = new ResourceManager();
	OBJHolder* modelInfo;
	TextureHolder* TextureInfo;
	GLuint meshIndexCount = 0;
	GLuint meshObjects = 0;
	TTF_Font * textFont;
	GLuint shaderProgram;
	GLuint textures[5];
	GLuint Buttons[5];
	Inventory *inventoryInfo;
	int invenSize = 0;
	PlayableCharacter * arnould;


	std::clock_t start;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	CombatInstance* combat;

	GLuint textToTexture(const char * str, GLuint textID, TTF_Font *font /*SDL_Color colour, GLuint &w,GLuint &h */);
	void loadRect(); //Drawing rectangles on screen

	void textBox(GLuint text, GLfloat y, bool drawBox, int nameText);
	int createTexture(const char * str, TTF_Font *font);

	void statusBar(GLfloat y, GLuint texture2, float health);
	void button(GLuint button, GLuint time);
	void inventory();
	void updateInven();
	//GLuint itemTextures[];

	vector<GLuint> itemTextures = vector<GLuint>();


	~UI();
};

