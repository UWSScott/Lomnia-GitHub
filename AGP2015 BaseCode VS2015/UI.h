#pragma once
#include <SDL_ttf.h>
#include "rt3d.h"
#include "ResourceManager.h"
#include "Gameobject.h"

class UI
{
public:
	UI();

	ResourceManager* Resource_Managment = new ResourceManager();
	OBJHolder* modelInfo;
	GLuint meshIndexCount = 0;
	GLuint meshObjects = 0;

	GLuint textToTexture(const char * str, GLuint textID, TTF_Font *font /*SDL_Color colour, GLuint &w,GLuint &h */);
	void loadRect(); //Drawing rectangles on screen

	void textBox(GLuint text, GLuint shader, GLfloat y, GLuint texture, bool drawBox, int nameText);
	int createTexture(const char * str, TTF_Font *font);

	void statusBar(GLuint shader, GLfloat y, GLuint texture, GLuint texture2, float health);
	void button(GLuint shader, GLuint texture, GLuint button, GLuint time);


	~UI();
};

