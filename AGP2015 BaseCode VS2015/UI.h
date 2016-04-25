#pragma once
#include <SDL_ttf.h>
#include "rt3d.h"
#include "ResourceManager.h"
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
	GLuint textures[6];
	GLuint Buttons[5];
	Inventory *inventoryInfo;
	int invenSize = 0;
	glm::mat4 stack;
	SDL_Event event;


	std::clock_t start;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	CombatInstance* combat;

	GLuint textToTexture(const char * str, GLuint textID/*, TTF_Font *font SDL_Color colour, GLuint &w,GLuint &h */);
	void loadRect(); //Drawing rectangles on screen

	void textBox(GLuint text, GLfloat y, bool drawBox, int nameText);
	int createTexture(const char * str);

	void statusBar(GLfloat y, GLuint texture2, float health);
	void button();
	int updateButton();
	void inventory(PlayableCharacter *arnould);
	void updateInven(PlayableCharacter *arnould);
	vector<int> itemTextures;

	GLuint arrowIndexCount = 0;
	GLuint arrowObjects = 0;

	GLuint questText[6];
	GLuint titles[5];

	void initQuests(PlayableCharacter *arnould);
	std::string currentQuest;

	void quest(PlayableCharacter *arnould);
	void QuestMarker(glm::mat4 object, GLuint shader, PlayableCharacter *arnould, vector<Character*> Game_Maze_Characters);
	float rotation = 0;

	void mainMenu();
	GLuint menuText[2];
	bool startMenu = true;
	bool quitMenu = false;

	void update();
	bool pressed = false;
	bool hub = false;
	bool triggerHub();


	~UI();
};

