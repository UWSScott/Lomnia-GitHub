// MD2 animation renderer
// This demo will load and render an animated MD2 model, an OBJ model and a skybox
// Most of the OpenGL code for dealing with buffer objects, etc has been moved to a 
// utility library, to make creation and display of mesh objects as simple as possible

//Edited by the IAN
//test for hayley
// Hello Chris

// Windows specific: Uncomment the following line to open a console window for debug output
#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

#include "FileLoader.h"
#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <stack>
#include "md2model.h"
#include <SDL_ttf.h>

#include <vector>
#include <iostream>
#include <windows.h>
#include <fstream>
#include "stdafx.h" //http://sourceforge.net/p/wpbdc/website/ci/master/tree/Judge/StdAfx.h
#include <list>

#include "Character.h"
#include "Weapon.h"
#include "PlayableCharacter.h"
#include "Camera.h"
#include "Skybox.h"
#include "Prefab.h"
#include "MazeGenerator.h"
#include "HeightMap.h"
#include "Terrain.h"
#include "ResourceManager.h"
#include "UI.h"
#include "EnemyType.h"

/*#include "Stun.h"
#include "Poison.h"
#include "ItemUse.h"
#include "Flee.h"
#include "HeavyAttack.h"
#include "LightAttack.h"
#include "Collisions.h"*/

//list<C_Attack> queuedAttacks = list<C_Attack>();

using namespace std;

#define DEG_TO_RADIAN 0.017453293

// Globals
// Real programs don't use globals :-D

GLuint meshIndexCount = 0;
GLuint md2VertCount = 0;
GLuint md2VertCount2 = 0;
GLuint md2VertCount3 = 0;
GLuint meshObjects[4];
bool pressedButton = false;
GLuint shaderProgram;
GLuint skyboxProgram;

enum GameStates { COMBAT, HUB, MAZE, MENU }; // Game states
GameStates gameState;
int gameStateInt = 0;

glm::vec2 enemyMove;

GLfloat playerRotation = 0.0f;
GLfloat r = 0.0f;

glm::vec3 eye(0.0f, 1.0f, 0.0f);
glm::vec3 at(0.0f, 1.0f, -1.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);

stack<glm::mat4> mvStack;

// TEXTURE STUFF
GLuint textures[7];
GLuint skybox[5];
GLuint labels[5];


//shop interface variables
bool shopMusicPlaying = false; 
bool bronzeSilverGoldOption = false; 
bool lightMediumStrongOption = false; 
bool buying = false; 
bool sellPotion = false; 
bool bossSpawn = false;
string item;



rt3d::lightStruct light0 = {
	{ 0.3f, 0.3f, 0.3f, 0.3f }, // ambient
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
	{ 0.5f, 0.5f, 0.5f, 0.5f }, // specular
	{ -5.0f, 4.0f, 2.0f, 1.0f }  // position
};
glm::vec4 lightPos(8.0f, 5.0f, -12.0f, 0.0f); //light position

rt3d::materialStruct material0 = {
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // ambient
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
	2.0f  // shininess
};
rt3d::materialStruct material1 = {
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // ambient
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
	1.0f  // shininess
};

// md2 stuff
md2model tmpModel;
md2model weapon;
md2model arnould;

glm::vec3 playerPos;
glm::vec3 enemyPos;
int currentAnim = 0;
int enemyAnim = 1;
int camera = 1;
float heightOfCam = 0;

TTF_Font * textFont;

UI * ui;
GLuint text[10];
GLuint names[10];
GLuint button[10];

const char *skyboxFiles[6] = {
	"red-sky/red_sky_right.bmp", "red-sky/red_sky_left.bmp", "red-sky/red_sky_back.bmp", "red-sky/red_sky_front.bmp", "red-sky/red_sky_top.bmp", "red-sky/red_sky_top.bmp"
};


glm::vec3 oldPlayerPos;

vector<Prefab> Game_Hub_Prefabs;
vector<Character*> Game_Hub_Characters;
vector<Character*> Game_Hub_Characters_Shop;
vector<Character*> Game_Maze_Characters;

ResourceManager* Resource_Managment = new ResourceManager();
Camera Game_Camera = Camera();
Character* static_character[15];
PlayableCharacter* character;// = new PlayableCharacter();
Prefab* shadow_Debug;// = new Gameobject()
Skybox* skyboxTest;
MazeGenerator* maze;
Prefab* houseTest;// = new Prefab();
Terrain* terrain;// = new Terrain();
Weapon* testWeapon;
//weapon = new Weapon("Scott's Saber", "Models/Partical_sword.MD2", "hobgoblin2.bmp", 0, 5, 5, "SWORD", 1, shaderProgram);
//weapon->setEquiped(true);

GLuint normalShadowProgram;
GLuint realShadowShader;
GLuint simpleDepthShader;
GLuint debugDepthQuad;
GLuint depthMapFBO = 0;
GLuint depthMap;
bool openShop;
const GLuint SHADOW_WIDTH = 7680, SHADOW_HEIGHT = 4320;
//const GLuint SHADOW_WIDTH = 1920, SHADOW_HEIGHT = 1080;
int currentPass = 0;
int frameRate = 0;
GLuint screenHeight = 1080;
GLuint screenWidth = 1920;

void SpawnFinalBoss()
{
	character->position = glm::vec3(16.6, 1.2, -19.1);
	character->rotation = -90;

	Game_Hub_Characters[4]->health = Game_Hub_Characters[4]->max_Health;
	Game_Hub_Characters[4]->enemy = true;
	Game_Hub_Characters[4]->canDraw = true;

	if (!character->inCombat)
	{
		character->EnterCombat(Game_Hub_Characters[4]);
		Game_Hub_Characters[4]->EnterCombat(character);
	}

	Game_Hub_Characters[0]->position = glm::vec3(9.41, 1.2, -10.14);
	Game_Hub_Characters[0]->rotation = 26;
	Game_Hub_Characters[1]->position = glm::vec3(19.7, 1.2, -9.6);
	Game_Hub_Characters[1]->rotation = 2;
	Game_Hub_Characters[2]->position = glm::vec3(24,1.2,-25);
	Game_Hub_Characters[2]->rotation = -152;
	Game_Hub_Characters[3]->position = glm::vec3(15, 1.2, -30);
	Game_Hub_Characters[3]->rotation = -192;
	Game_Hub_Characters[5]->position = glm::vec3(4, 1.2, -27);
	Game_Hub_Characters[5]->rotation = -240;
	Game_Hub_Characters[6]->position = glm::vec3(3.7,1.2,-18.6);
	Game_Hub_Characters[6]->rotation = -271;
	Game_Hub_Characters[7]->position = glm::vec3(23, 1.2, -10);
	Game_Hub_Characters[7]->rotation = -379;
	Game_Hub_Characters[8]->position = glm::vec3(29,1.2,-18);
	Game_Hub_Characters[8]->rotation = -439;

	/*Game_Hub_Characters.push_back(new Character("AI_1", Resource_Managment->LoadMD2("Models/ripper.MD2"), Resource_Managment->LoadTexture("Models/Textures/Bronze_Skin.bmp"), glm::vec3(1), glm::vec3(50, 1.2, -30), shaderProgram));
	Game_Hub_Characters.push_back(new Character("AI_2", Resource_Managment->LoadMD2("Models/quigon.MD2"), Resource_Managment->LoadTexture("Models/Textures/Chris_Skin.bmp"), glm::vec3(1), glm::vec3(30, 1.2, 20), shaderProgram));
	Game_Hub_Characters.push_back(new Character("AI_3", Resource_Managment->LoadMD2("Models/pogo_buny.MD2"), Resource_Managment->LoadTexture("Models/Textures/Chain_Link.bmp"), glm::vec3(1), glm::vec3(20, 1.2, 25), shaderProgram));
	Game_Hub_Characters.push_back(new Character("AI_4", Resource_Managment->LoadMD2("Models/dragon.MD2"), Resource_Managment->LoadTexture("Models/Textures/Bronze_Skin.bmp"), glm::vec3(1), glm::vec3(15, 1.2, 20), shaderProgram));
	Game_Hub_Characters.push_back(new Character("END_BOSS", Resource_Managment->LoadMD2("Models/faerie.MD2"), Resource_Managment->LoadTexture("Models/Textures/Chain_Link.bmp"), glm::vec3(1), glm::vec3(13, 1.8, -19.7), shaderProgram));
	Game_Hub_Characters.push_back(new Character("AI_6", Resource_Managment->LoadMD2("Models/ogro.MD2"), Resource_Managment->LoadTexture("Models/Textures/White_Fur.bmp"), glm::vec3(1), glm::vec3(30, 1.2, -10), shaderProgram));
	Game_Hub_Characters.push_back(new Character("AI_7", Resource_Managment->LoadMD2("Models/centaur.MD2"), Resource_Managment->LoadTexture("Models/Textures/Blue_Leather.bmp"), glm::vec3(1), glm::vec3(30, 1.2, 10), shaderProgram));
	Game_Hub_Characters.push_back(new Character("Blade", Resource_Managment->LoadMD2("Models/blade.MD2"), Resource_Managment->LoadTexture("Models/Textures/Gold_Skin.bmp"), glm::vec3(1), glm::vec3(5, 1.2, -10), shaderProgram));
	Game_Hub_Characters.push_back(new Character("AI_9", Resource_Managment->LoadMD2("Models/zf19.MD2"), Resource_Managment->LoadTexture("Models/Textures/Gold_Skin.bmp"), glm::vec3(1), glm::vec3(15, 1.2, -10), shaderProgram));*/

}

// textToTexture
GLuint textToTexture(const char * str, GLuint textID/*, TTF_Font *font, SDL_Color colour, GLuint &w,GLuint &h */) {
	TTF_Font *font = textFont;
	SDL_Color colour = { 255, 255, 255 };
	SDL_Color bg = { 0, 0, 0 };

	SDL_Surface *stringImage;
	stringImage = TTF_RenderText_Blended(font, str, colour);

	if (stringImage == NULL)
		//exitFatalError("String surface not created.");
		std::cout << "String surface not created." << std::endl;

	GLuint w = stringImage->w;
	GLuint h = stringImage->h;
	GLuint colours = stringImage->format->BytesPerPixel;

	GLuint format, internalFormat;
	if (colours == 4) {   // alpha
		if (stringImage->format->Rmask == 0x000000ff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	}
	else {             // no alpha
		if (stringImage->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	}
	internalFormat = (colours == 4) ? GL_RGBA : GL_RGB;

	GLuint texture = textID;

	if (texture == 0) {
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	} //Do this only when you initialise the texture to avoid memory leakage

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, stringImage->w, stringImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, stringImage->pixels);
	glBindTexture(GL_TEXTURE_2D, NULL);

	SDL_FreeSurface(stringImage);
	return texture;
}

// SAVE MAZE -- Cell Level[][SIZE]


// Set up rendering context
SDL_Window * setupRC(SDL_GLContext &context) {
	SDL_Window * window;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
		rt3d::exitFatalError("Unable to initialize SDL");

	// Request an OpenGL 3.0 context.

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 8 bit alpha buffering
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Turn on x4 multisampling anti-aliasing (MSAA)

													   // Create 800x600 window 
	window = SDL_CreateWindow("SDL/GLM/OpenGL Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) // Check window was created OK
		rt3d::exitFatalError("Unable to create window");

	context = SDL_GL_CreateContext(window); // Create opengl context and attach to window
	SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
	return window;
}

// A simple texture loading function
// lots of room for improvement - and better error checking!
GLuint loadBitmap(char *fname) {
	GLuint texID;
	glGenTextures(1, &texID); // generate texture ID

							  // load file - using core SDL library
	SDL_Surface *tmpSurface;
	tmpSurface = SDL_LoadBMP(fname);
	if (!tmpSurface) {
		std::cout << "Error loading bitmap" << fname << std::endl;
		system("Pause");
	}

	// bind texture and set parameters
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	SDL_PixelFormat *format = tmpSurface->format;

	GLuint externalFormat, internalFormat;
	if (format->Amask) {
		internalFormat = GL_RGBA;
		externalFormat = (format->Rmask < format->Bmask) ? GL_RGBA : GL_BGRA;
	}
	else {
		internalFormat = GL_RGB;
		externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tmpSurface->w, tmpSurface->h, 0,
		externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(tmpSurface); // texture loaded, free the temporary buffer
	return texID;	// return value of texture ID
}

void CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;       // This will store our fps
	static float lastTime = 0.0f;       // This will hold the time from the last frame
	float currentTime = GetTickCount() * 0.001f;
	++framesPerSecond;
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
	//	if (1 == 1) fprintf(stderr, "\nCurrent Frames Per Second: %d\n\n", (int)framesPerSecond);
		frameRate = (int)framesPerSecond;
		framesPerSecond = 0;
	}
}


void StartGame()
{
	character->currentQuest->status = 1;
	character->status = STATE_MAZE;
	gameState == MAZE;
	maze->EnterTheMazetrix(character, Resource_Managment);
}

void init(void)
{
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << " ERROR ;";

	gameState = MENU; // Game starts in hub area by default

	shaderProgram = rt3d::initShaders("phong-tex.vert", "phong-tex.frag");
	rt3d::setLight(shaderProgram, light0);
	rt3d::setMaterial(shaderProgram, material0);
	skyboxProgram = rt3d::initShaders("textured.vert", "textured.frag");
	normalShadowProgram = rt3d::initShaders("realTimeShadow.vert", "realTimeShadow.frag");
	glUniform1i(glGetUniformLocation(normalShadowProgram, "diffuseTexture"), 1);
	glUniform1i(glGetUniformLocation(normalShadowProgram, "shadowMap"), 0);
	simpleDepthShader = rt3d::initShaders("shadow.vert", "shadow.frag");
	ui = new UI;

	if (TTF_Init() == -1)
		cout << "TTF failed to initialise." << endl;

	textFont = TTF_OpenFont("ESKARGOT.ttf", 48);
	if (textFont == NULL)
		cout << "Failed to open font." << endl;


	textures[2] = loadBitmap("Models/Textures/textbox.bmp");
	textures[3] = loadBitmap("Models/Textures/health texture.bmp");
	textures[4] = loadBitmap("Models/Textures/StatusBar.bmp");
	textures[5] = loadBitmap("Models/Textures/mana texture.bmp");

	ui->loadRect();
	text[0] = ui->createTexture("Ahh, the mighty Arnould the wild!");
	text[1] = ui->createTexture("Your challenger this time is the raging beast known as fred!");
	text[2] = ui->createTexture("Welcome to the shop.");
	names[0] = ui->createTexture("Ian:");
	button[0] = ui->createTexture("G");

	/*static_character[0] = new Character("Arnold", "Models/walker.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(1, 0, 0), shaderProgram);
	static_character[1] = new Character("Arnold", "Models/ddz.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(3, 0, 0), shaderProgram);
	static_character[2] = new Character("Arnold", "Models/blade.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(5, 0, 0), shaderProgram);
	static_character[3] = new Character("Arnold", "Models/centaur.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(7, 0, 0), shaderProgram);
	static_character[4] = new Character("Arnold", "Models/ripper.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(9, 0, 0), shaderProgram);
	static_character[5] = new Character("Arnold", "Models/ogro.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(11, 0, 0), shaderProgram);
	static_character[6] = new Character("Arnold", "Models/ogro_wepaon.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(11, 0, 0), shaderProgram);
	static_character[7] = new Character("Arnold", "Models/dragon.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(15, 0, 0), shaderProgram);
	static_character[8] = new Character("Arnold", "Models/zf19.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(20, 0, 0), shaderProgram);
	static_character[9] = new Character("Arnold", "Models/faerie.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(22, 0, 0), shaderProgram);
	static_character[10] = new Character("Arnold", "Models/frosty.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(24, 0, 0), shaderProgram);
	static_character[11] = new Character("Arnold", "Models/pogo_buny.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(26, 0, 0), shaderProgram);
	static_character[12] = new Character("Arnold", "Models/quigon.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(28, 0, 0), shaderProgram);*/

	skyboxTest = new Skybox(skyboxFiles);
	maze = new MazeGenerator(shaderProgram, *Resource_Managment);
	character = new PlayableCharacter("Arnold", "Models/arnould.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(10, 1.2, 10), shaderProgram);
	Game_Camera.InitalStats();
	character->InitalStats(shaderProgram);

	testWeapon = new Weapon("Scott's Saber", "Models/Partical_sword.MD2", "hobgoblin2.bmp", 0, 5, 5, "SWORD", 1, shaderProgram);
	testWeapon->setEquiped(true);
	character->weapon = testWeapon;

	houseTest = new Prefab(shaderProgram, "cube.obj", "Models/Textures/Terrain_Sand.bmp", glm::vec3(500.0, 0.1, 500.0), glm::vec3(0, 0, 0));
	terrain = new Terrain(shaderProgram, "Models/Desert_Terrain_New_Low.obj", "Models/Textures/Terrain_Sand.bmp", glm::vec3(1, 1, 1), glm::vec3(300, 0, -300), 0);

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Corner_Ruin.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(0.03, 0.02, 0.03), glm::vec3(-61.1, 0.49, -41.55), -110, glm::vec3(-62, 6.8, -35), glm::vec3(-65.5, -1.0, -42)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Corner_Ruin.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(0.03, 0.02, 0.03), glm::vec3(3.3, 0.49, 71.2), -110, glm::vec3(1.6, 6.8, 77), glm::vec3(-3, -1.0, 69)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Corner_Ruin.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(0.03, 0.02, 0.03), glm::vec3(89.5, 0.49, 83.26), -173.1, glm::vec3(90, 6.8, 82.5), glm::vec3(83.5, -1.0, 79)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Corner_Ruin.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(0.03, 0.02, 0.03), glm::vec3(86.8, 0.49, -65.8), 173.14, glm::vec3(88, 6.8, -66), glm::vec3(80.3, -1.0, -72)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Corner_Ruin.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(0.03, 0.02, 0.03), glm::vec3(128.7, 2.67, -54.8), 173.14, glm::vec3(130, 6.8, -54.8), glm::vec3(122, -1.0, -60)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Corner_Ruin.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(0.03, 0.02, 0.03), glm::vec3(20.12, 0.49, -72.95), 17.14, glm::vec3(26, 6.8, -68), glm::vec3(21, -1.0, -74)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Corner_Ruin.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(0.03, 0.02, 0.03), glm::vec3(22.43, 0.49, -59.3), 97.2, glm::vec3(27, 6.8, -59), glm::vec3(21, -1.0, -64)));

	////////////////////////////////////
	///Arnoulds house look at later/////
	////////////////////////////////////

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_Player.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(2.5, 2, 2.5), glm::vec3(47.29, 2.54, 82.97), -123.7, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_Player.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(2.5, 2, 2.5), glm::vec3(-15.6, 2.54, -87.2), -123.7, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0)));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Shop_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Shop_001.bmp"), glm::vec3(0.7, 0.7, 0.7), glm::vec3(-49.1, 0, -46.89), -99, glm::vec3(-44, 5.7, -45.5), glm::vec3(-52, -1.0, -50)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Shop_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Shop_001.bmp"), glm::vec3(0.7, 0.7, 0.7), glm::vec3(19.1, 0, 70.05), 0, glm::vec3(20, 5.7, 73), glm::vec3(16, -1.0, 66.5)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Shop_002.obj"), Resource_Managment->LoadTexture("Models/Textures/Shop_001.bmp"), glm::vec3(1.3, 1.3, 1.3), glm::vec3(104.1, 0, -9.9), -90, glm::vec3(108, 3.8, -7.4), glm::vec3(102.5, -1.0, -9.8)));

	
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Well.obj"), Resource_Managment->LoadTexture("Models/Textures/Well.bmp"), glm::vec3(2.0, 2.0, 2.0), glm::vec3(61.1, 1.25, 0), 30, glm::vec3(64.7, 3.1, 3), glm::vec3(58.5, -1.0, -3)));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Teleporter.obj"), Resource_Managment->LoadTexture("Models/Textures/Well.bmp"), glm::vec3(0.4, 0.4, 0.4), glm::vec3(39, 0.36, 0), 0, glm::vec3(37.4, 2.0, 2.4), glm::vec3(34.4, -1.0, -2.5)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Teleporter_Stand.obj"), Resource_Managment->LoadTexture("Models/Textures/Well.bmp"), glm::vec3(0.4, 0.4, 0.4), glm::vec3(39.33, 0.01, 2.52), 0, glm::vec3(39, 3.2, 4.5), glm::vec3(36.4, -1.0, 2.8)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Teleporter_Stand.obj"), Resource_Managment->LoadTexture("Models/Textures/Well.bmp"), glm::vec3(0.4, 0.4, 0.4), glm::vec3(34.6, 0.01, -2.1), 0, glm::vec3(35.3, 3.2, 0.0), glm::vec3(32.6, -1.0, -4)));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Teleporter_Stand.obj"), Resource_Managment->LoadTexture("Models/Textures/Well.bmp"), glm::vec3(5, 5, 5), glm::vec3(-149.33, 1.86, 77.7), 0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Teleporter_Stand.obj"), Resource_Managment->LoadTexture("Models/Textures/Well.bmp"), glm::vec3(10, 10, 10), glm::vec3(15.5, 1.86, -126), 0, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0)));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Well.bmp"), glm::vec3(0.02, 0.02, 0.02), glm::vec3(-42.3, 0, -18.1), -22, glm::vec3(-33.6, 10.0, -12), glm::vec3(-49, -1.0, -27)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Well.bmp"), glm::vec3(0.02, 0.02, 0.02), glm::vec3(-22.16, 0, -39.8), -49, glm::vec3(-12, 10.0, -35), glm::vec3(-49, -1.0, -49)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Well.bmp"), glm::vec3(0.02, 0.02, 0.02), glm::vec3(52.4, 0, -48.6), 23.4, glm::vec3(60.5, 10.0, -40), glm::vec3(44, -1.0, -57)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Well.bmp"), glm::vec3(0.02, 0.02, 0.02), glm::vec3(54.5, 0, 43.9), -28.2, glm::vec3(63, 10.0, 53), glm::vec3(46, -1.0, 36)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Well.bmp"), glm::vec3(0.02, 0.02, 0.02), glm::vec3(0, 0, 54.1), 19.4, glm::vec3(5.8, 10.0, 62), glm::vec3(-6.4, -1.0, 44)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Well.bmp"), glm::vec3(0.02, 0.02, 0.02), glm::vec3(121.1, 0, -28.1), 23.4, glm::vec3(129, 10.0, -20), glm::vec3(113, -1.0, -36)));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Destroyed_House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Rusty_Shed_Metal_Texture.bmp"), glm::vec3(1.0, 1.0, 1.0), glm::vec3(151, -2.51, -22), -149, glm::vec3(158, 20, -17), glm::vec3(143.5, -1.0, -28)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Destroyed_House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Rusty_Shed_Metal_Texture.bmp"), glm::vec3(1.0, 1.0, 1.0), glm::vec3(163, -0.64, -4.3), -72, glm::vec3(171, 20, 4.5), glm::vec3(156, -1.0, -12)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Destroyed_House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Rusty_Shed_Metal_Texture.bmp"), glm::vec3(1.0, 1.0, 1.0), glm::vec3(-51.9, 0, -75), -83, glm::vec3(-43, 20, -66.5), glm::vec3(-60.4, -1.0, -84)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Destroyed_House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Rusty_Shed_Metal_Texture.bmp"), glm::vec3(1.0, 1.0, 1.0), glm::vec3(67.6, 0, -69.4), -72, glm::vec3(74, 20, -60), glm::vec3(59, -1.0, -77)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Destroyed_House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Rusty_Shed_Metal_Texture.bmp"), glm::vec3(1.0, 1.0, 1.0), glm::vec3(110, 0, 20), -30.4, glm::vec3(117, 20, 27.5), glm::vec3(102, -1.0, 11)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Destroyed_House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Rusty_Shed_Metal_Texture.bmp"), glm::vec3(1.0, 1.0, 1.0), glm::vec3(21.4, 0, 96.3), 0, glm::vec3(30, 20, 104.5), glm::vec3(12.5, -1.0, 88)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Destroyed_House_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Rusty_Shed_Metal_Texture.bmp"), glm::vec3(1.0, 1.0, 1.0), glm::vec3(0, 0, 0), -47.9, glm::vec3(5.6, 20, 10), glm::vec3(-9, -1.0, -9)));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_002.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(60.0, 60.0, 60.0), glm::vec3(17, 0, -88), -205, glm::vec3(23.8, 6.6, -80), glm::vec3(9.1, -1.0, -96.5)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_002.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(60.0, 60.0, 60.0), glm::vec3(144.5, 1.07, 17.43), 39.4, glm::vec3(156, 6.6, 23.4), glm::vec3(134.8, -1.0, 8.5)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_002.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(60.0, 60.0, 60.0), glm::vec3(-68.3, 0, -18), -93.5, glm::vec3(-60, 6.6, -8.5), glm::vec3(-77, -1.0, -26)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_002.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(60.0, 60.0, 60.0), glm::vec3(62.9, 0, 61.5), -64.4, glm::vec3(70.4, 6.6, 72), glm::vec3(54, -1.0, 51.7)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_002.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(60.0, 60.0, 60.0), glm::vec3(86.5, 0, 58.8), 39.4, glm::vec3(96, 6.6, 68), glm::vec3(76, -1.0, 48)));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/House_002.obj"), Resource_Managment->LoadTexture("Models/Textures/House_002.bmp"), glm::vec3(60.0, 60.0, 60.0), glm::vec3(97.11, 0, -50.2), 150, glm::vec3(106, 6.6, -40), glm::vec3(87.6, -1.0, -60)));


	// Collectable item
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, Resource_Managment->LoadObject("Models/Loot_Drop_001.obj"), Resource_Managment->LoadTexture("Models/Textures/Texture1.bmp"), glm::vec3(0.01, 0.01, 0.01), glm::vec3(13, 0.36, 0), 0, glm::vec3(15, 5, 3), glm::vec3(5, -3, -3)));
	Game_Hub_Characters.push_back(new Character("AI_1", Resource_Managment->LoadMD2("Models/ripper.MD2"), Resource_Managment->LoadTexture("Models/Textures/Bronze_Skin.bmp"), glm::vec3(1), glm::vec3(50, 1.2, -30), shaderProgram));
	Game_Hub_Characters.push_back(new Character("AI_2", Resource_Managment->LoadMD2("Models/quigon.MD2"), Resource_Managment->LoadTexture("Models/Textures/Chris_Skin.bmp"), glm::vec3(1), glm::vec3(30, 1.2, 20), shaderProgram));
	Game_Hub_Characters.push_back(new Character("AI_3", Resource_Managment->LoadMD2("Models/pogo_buny.MD2"), Resource_Managment->LoadTexture("Models/Textures/Chain_Link.bmp"), glm::vec3(1), glm::vec3(20, 1.2, 25), shaderProgram));
	Game_Hub_Characters.push_back(new Character("AI_4", Resource_Managment->LoadMD2("Models/dragon.MD2"), Resource_Managment->LoadTexture("Models/Textures/Bronze_Skin.bmp"), glm::vec3(1), glm::vec3(15, 1.2, 20), shaderProgram));
	Game_Hub_Characters.push_back(new Character("END_BOSS", Resource_Managment->LoadMD2("Models/faerie.MD2"), Resource_Managment->LoadTexture("Models/Textures/God_World.bmp"), glm::vec3(1), glm::vec3(13, 1.8, -19.7), shaderProgram));
	Game_Hub_Characters.push_back(new Character("ORGO", Resource_Managment->LoadMD2("Models/ogro.MD2"), Resource_Managment->LoadTexture("Models/Textures/White_Fur.bmp"), glm::vec3(1), glm::vec3(30, 1.2, -10), shaderProgram));
	Game_Hub_Characters.push_back(new Character("AI_7", Resource_Managment->LoadMD2("Models/centaur.MD2"), Resource_Managment->LoadTexture("Models/Textures/Blue_Leather.bmp"), glm::vec3(1), glm::vec3(30, 1.2, 10), shaderProgram));
	Game_Hub_Characters.push_back(new QuestGiver(Resource_Managment->LoadMD2("Models/blade.MD2"), Resource_Managment->LoadTexture("Models/Textures/Gold_Skin.bmp"), glm::vec3(1), glm::vec3(5, 1.2, -10), shaderProgram));
	Game_Hub_Characters.push_back(new Character("AI_9", Resource_Managment->LoadMD2("Models/zf19.MD2"), Resource_Managment->LoadTexture("Models/Textures/Gold_Skin.bmp"), glm::vec3(1), glm::vec3(15, 1.2, -10), shaderProgram));
	Game_Hub_Characters[7]->enemy = false;
	//Game_Hub_Characters[4]->enemy = true;
	//Game_Hub_Characters[4]->health = -1;

	Game_Hub_Characters[4]->enemy = true;
	Game_Hub_Characters[4]->canDraw = false;
	Game_Hub_Characters[4]->health = -1;
	Game_Hub_Characters[4]->rotation = 90;

	//shop guys
	Game_Hub_Characters.push_back(new Character("AI_10", Resource_Managment->LoadMD2("Models/ogro.MD2"), Resource_Managment->LoadTexture("Models/Textures/Gold_Skin.bmp"), glm::vec3(1), glm::vec3(18, 1.3, 71.5), shaderProgram));
	Game_Hub_Characters.push_back(new Character("AI_11", Resource_Managment->LoadMD2("Models/ogro.MD2"), Resource_Managment->LoadTexture("Models/Textures/Gold_Skin.bmp"), glm::vec3(0.8), glm::vec3(106, 1.1, -8.1), shaderProgram));
	Game_Hub_Characters[10]->rotation = -93;

	//Game_Maze_Characters.push_back(new Minion("MAZE_AI_1", Resource_Managment->LoadMD2("Models/ripper.MD2"), Resource_Managment->LoadTexture("Models/Textures/Bronze_Skin.bmp"), glm::vec3(1), glm::vec3(50, 1.2, -30), shaderProgram));
	//Game_Maze_Characters.push_back(new Minion("MAZE_AI_2", Resource_Managment->LoadMD2("Models/ogro.MD2"), Resource_Managment->LoadTexture("Models/Textures/Bronze_Skin.bmp"), glm::vec3(1), glm::vec3(50, 1.2, -30), shaderProgram));
	//Game_Maze_Characters.push_back(new Minion("MAZE_AI_3", Resource_Managment->LoadMD2("Models/pogo_buny.MD2"), Resource_Managment->LoadTexture("Models/Textures/Bronze_Skin.bmp"), glm::vec3(1), glm::vec3(50, 1.2, -30), shaderProgram));
	//Game_Maze_Characters.push_back(new Minion("MAZE_AI_4", Resource_Managment->LoadMD2("Models/quigon.MD2"), Resource_Managment->LoadTexture("Models/Textures/Bronze_Skin.bmp"), glm::vec3(1), glm::vec3(50, 1.2, -30), shaderProgram));


	//NPCs in the hub areaw
	//Game_Hub_Characters.push_back(Character("Arnold", "Models/arnould.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(1, 0, 0), shaderProgram));

	//NPCS that serve a purpose (shop owners/traders) 
	//Game_Hub_Characters_Shop.push_back(Character("Arnold", "Models/arnould.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(-4, 2, -4), shaderProgram));

	lightPos.x = 60;
	lightPos.y = 100;
	lightPos.z = -25;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//StartGame();
}




glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d) {
	return glm::vec3(pos.x + d*std::sin(r*DEG_TO_RADIAN), pos.y, pos.z - d*std::cos(r*DEG_TO_RADIAN));
}

glm::vec3 moveRight(glm::vec3 pos, GLfloat angle, GLfloat d) {
	return glm::vec3(pos.x + d*std::cos(r*DEG_TO_RADIAN), pos.y, pos.z + d*std::sin(r*DEG_TO_RADIAN));
}
//ttt

void update(void) {
	CalculateFrameRate();
	
	//cout << " arn pos: " << character->position.x << "  y: " << character->position.y << " z: " << character->position.z << " rotation: " << character->rotation << endl;
	gameStateInt = gameState;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	Game_Camera.update(character->getModelEye(), character->getRotation());
	character->Update(frameRate, &Game_Camera);
	character->Collider->CollisionCircles((GLfloat)character->position.x, (GLfloat)character->position.z, 0.5);

	if (character->health < 1)
	{
		if (character->currentQuest->name == "Free Lomnia")
		{
			//Game over - Credits
		}
		else
		{
			character->Respawn();
			gameState = HUB;
		}
	}

	if (gameState == HUB)
	{
		if (character->currentQuest->name == "Free Lomnia")
		{
			if (bossSpawn == false)
			{
				SpawnFinalBoss();
				bossSpawn = true;
			}
		}
		for (int i = 0; i < Game_Hub_Characters.size(); i++)
		{
			//Game_Hub_Characters[i]->RotateToFace(character);
			Game_Hub_Characters[i]->Update(frameRate);
		}

		for (int i = 0; i < Game_Hub_Prefabs.size(); i++)
		{
			for (int j = 0; j < Game_Hub_Characters.size(); j++)
			{
				if (j != 9 && j!=10) // so shop keepers can be drawn inside shops, dont check them for collision with buildings
				{
					if (Game_Hub_Characters[j]->Collider->checkCollision(Game_Hub_Prefabs[i].Collider->aabb, Game_Hub_Characters[j]->position))
					{
						Game_Hub_Characters[j]->position = Game_Hub_Characters[j]->oldPosition;
					}
				}
			}
		}

		for (int i = 0; i < Game_Hub_Prefabs.size(); i++)
		{
			if (character->Collider->checkCollision(Game_Hub_Prefabs[i].Collider->aabb, character->position))
			{
				character->position = character->oldPosition; //oldPlayerPos;
				character->CheckCollision(&Game_Hub_Prefabs[i], typeid(Game_Hub_Prefabs[i]).name());
				cout << "collision with " << i << endl;

				if (i == 13 && character->currentQuest != NULL && character->currentQuest->status == 0) // if teleporter
				{
					gameState = MAZE;
					gameStateInt = gameState;
					maze->EnterTheMazetrix(character, Resource_Managment);
					character->status = STATE_MAZE;
					return;
					//maze->SpawnCharacter(character);
					//for (int i = 0; i < Game_Maze_Characters.size(); i++)
					//{
					//	maze->SpawnCharacter(Game_Maze_Characters[i]);
					//}

				}
				if (i == 37) // if item
				{
					character->inventory->AddRandomItem();
					character->inventory->show();
					Game_Hub_Prefabs.pop_back();
				}
				if (i == 10) // colliding with shop
				{
					names[0] = ui->createTexture("Buy:");
					openShop = true;
					buying = true;
				}
				else if (i == 11)
				{
					names[0] = ui->createTexture("Sell:");
					openShop = true;
				}
				else
				{
					openShop = false;
				}

			}
		}

		for (int i = 0; i < Game_Hub_Characters.size(); i++)
		{

			Game_Hub_Characters[i]->Collider->CollisionCircles((GLfloat)Game_Hub_Characters[i]->position.x, (GLfloat)Game_Hub_Characters[i]->position.z, 0.5);

			if (character->Collider->checkCollision(character->Collider, Game_Hub_Characters[i]->Collider))
			{
				character->position = character->oldPosition;
				character->CheckCollision(Game_Hub_Characters[i], typeid(Game_Hub_Characters[i]).name());

				character->CheckQuestGoal(Game_Hub_Characters[i]);
			}

		}


		//REMOVE!!!!!!5
		/*for (int j = 0; j < Game_Hub_Characters.size(); j++)
		{
			Game_Hub_Characters[j]->detector->CollisionCircles((GLfloat)Game_Hub_Characters[j]->position.x, (GLfloat)Game_Hub_Characters[j]->position.z, 20);

			if (Game_Hub_Characters[j]->detector->checkCollision(Game_Hub_Characters[j]->detector, character->Collider) && character->status != STATE_COMBAT)
			{
				Game_Hub_Characters[j]->MoveToPlayer(character);
			}
		}*/
		//END!!!!!!!!!!!!

	}

	if (openShop)
	{
		bool soldItem; 
		//character->inventory->show();
		if (shopMusicPlaying == false)
		{
			Game_Camera.SetPlayerStatus(3, character);
			character->status = STATE_SHOP;
			shopMusicPlaying = true;
		}

			Game_Hub_Characters[9]->position[2] = 74;
			Game_Hub_Characters[10]->position[0] = 105.5;
			Game_Hub_Characters[10]->position[1] = 1.9;
			Game_Hub_Characters[10]->position[2] = -4;
			//107, 1.1, -8.2
			
			if (bronzeSilverGoldOption)
			{

				text[2] = ui->createTexture("1. Bronze 2. Silver 3. Gold ");


				if (keys[SDL_SCANCODE_1]) // if bronze
				{
					if (buying)
					{
						character->inventory->buyItem(item, RARITY_BRONZE);
					}
					else
					{
						character->inventory->sellItem(item, RARITY_BRONZE);
					}

					openShop = false;
				}
				else if (keys[SDL_SCANCODE_2]) // if silver
				{
					if (buying)
					{
						character->inventory->buyItem(item, RARITY_SILVER);
					}
					else
					{
						character->inventory->sellItem(item, RARITY_SILVER);
					}
					openShop = false;
				}
				else if (keys[SDL_SCANCODE_3]) // if gold
				{
					if (buying)
					{
						character->inventory->buyItem(item, RARITY_GOLD);
					}
					else
					{
						character->inventory->sellItem(item, RARITY_GOLD);
					}

					openShop = false;
				}
			}

			if (lightMediumStrongOption)
			{

				text[2] = ui->createTexture("1. Light 2. Medium 3. Strong");
				if (keys[SDL_SCANCODE_1]) // if light
				{
					if (buying)
					{
						character->inventory->buyItem(item, POTION_POTENCY_LIGHT);
					}
					else
					{
						character->inventory->sellItem(item, POTION_POTENCY_LIGHT);
					}

					openShop = false;
				}
				else if (keys[SDL_SCANCODE_2]) // if medium
				{
					if (buying)
					{
						character->inventory->buyItem(item, POTION_POTENCY_MEDIUM);
					}
					else
					{
						character->inventory->sellItem(item, POTION_POTENCY_MEDIUM);
					}
					openShop = false;
				}
				else if (keys[SDL_SCANCODE_3]) // if strong
				{
					if (buying)
					{
						character->inventory->buyItem(item, POTION_POTENCY_STRONG);
					}
					else
					{
						character->inventory->sellItem(item, POTION_POTENCY_STRONG);
					}

					openShop = false;
				}

			}

			if (openShop)
			{
				if (!bronzeSilverGoldOption && !lightMediumStrongOption)
				{
					text[2] = ui->createTexture("1. Health Potion 2. Mana Potion 3. Sword 4.Axe 5.Knives");

					if (keys[SDL_SCANCODE_1]) // if health potion
					{

						lightMediumStrongOption = true;
						item = "HealthPotion";
					}
					else if (keys[SDL_SCANCODE_2]) // if mana potion
					{
						lightMediumStrongOption = true;
						item = "ManaPotion";

					}
					else if (keys[SDL_SCANCODE_3]) // if sword
					{
						bronzeSilverGoldOption = true;
						item = "Sword";

					}
					else if (keys[SDL_SCANCODE_4]) // if axe
					{
						bronzeSilverGoldOption = true;
						item = "Axe";
					}
					else if (keys[SDL_SCANCODE_5]) //if knives
					{
						bronzeSilverGoldOption = true;
						item = "Knives";
					}
					else if (keys[SDL_SCANCODE_ESCAPE]) // closes shop if Esc key is pressed
					{
						openShop = false;
					}

				}

			}
			if (openShop == false) // if shop closed, stop music and return camera to normal 
			{
				character->status = STATE_NORMAL;
				shopMusicPlaying = false;
				lightMediumStrongOption = false;
				Game_Hub_Characters[9]->position[2] = 71;

				Game_Hub_Characters[10]->position[0] = 106;
				Game_Hub_Characters[10]->position[2] = -8.1;
				Game_Hub_Characters[10]->position[1] = 1.1;
				buying = false;
			}
		
	}
	else if (gameState == COMBAT)
	{
		for (int i = 0; i < Game_Hub_Characters.size(); i++)
		{
			Game_Hub_Characters[i]->Update(frameRate);
		}

	}
	else if (gameState == MAZE)
	{
	/*	for (int j = 0; j < Game_Hub_Characters.size(); j++)
		{
			if (Game_Hub_Characters[j]->detector->checkCollision(Game_Hub_Characters[j]->detector, character->detector))
			{
				glm::lookAt(Game_Hub_Characters[j]->position, character->position, up);
			}
		}

		for (int j = 0; j < Game_Maze_Characters.size(); j++)
		{
			if (Game_Maze_Characters[j]->health > 0)
			{
				Game_Maze_Characters[j]->detector->CollisionCircles((GLfloat)Game_Maze_Characters[j]->position.x, (GLfloat)Game_Maze_Characters[j]->position.z, 20);

				if (Game_Maze_Characters[j]->detector->checkCollision(Game_Maze_Characters[j]->detector, character->Collider) && character->status != STATE_COMBAT)
				{
					Game_Maze_Characters[j]->MoveToPlayer(character);
				}
			}
		}

		for (int i = 0; i < Game_Maze_Characters.size(); i++)
		{
			if (Game_Maze_Characters[i]->health > 0)
			{
				Game_Maze_Characters[i]->Collider->CollisionCircles((GLfloat)Game_Maze_Characters[i]->position.x, (GLfloat)Game_Maze_Characters[i]->position.z, 0.5);

				if (character->Collider->checkCollision(character->Collider, Game_Maze_Characters[i]->Collider))
				{
					character->position = character->oldPosition;
					character->CheckCollision(Game_Maze_Characters[i], typeid(Game_Maze_Characters[i]).name());
					character->CheckQuestGoal(Game_Maze_Characters[i]);
				}
			}

		}

		for (int i = 0; i < Game_Maze_Characters.size(); i++)
		{
			if (Game_Maze_Characters[i]->health > 0)
				Game_Maze_Characters[i]->Update();
		}*/
		gameStateInt = gameState;
		//cout << " game state before: " << gameStateInt << endl;
		maze->Update(character, &Game_Camera, gameStateInt, frameRate);
		//cout << " game state after: " << gameStateInt << endl;
		if (gameStateInt == 1)
			gameState = HUB;
		else if(gameStateInt == 2)
			gameState = MAZE;
		else
			gameState = COMBAT;
	}

	character->oldPosition = character->position;
	for (int i = 0; i < Game_Hub_Characters.size(); i++)
	{
		Game_Hub_Characters[i]->oldPosition = Game_Hub_Characters[i]->position;
	}

	//if (keys[SDL_SCANCODE_R]) character->position.y += 0.1f;
	//if (keys[SDL_SCANCODE_F]) character->position.y -= 0.1f;
	//if (keys[SDL_SCANCODE_I]) lightPos.x += 0.1f;
	//if (keys[SDL_SCANCODE_K]) lightPos.x -= 0.1f;
	//if (keys[SDL_SCANCODE_J]) lightPos.z += 0.1f;
	//if (keys[SDL_SCANCODE_L]) lightPos.z -= 0.1f;
	//if (keys[SDL_SCANCODE_O]) lightPos.y += 0.1f;
	//if (keys[SDL_SCANCODE_P]) lightPos.y -= 0.1f;
	//if (keys[SDL_SCANCODE_N]) character->inventory->AddRandomItem();
	if (keys[SDL_SCANCODE_Y]) SpawnFinalBoss();
	//if (keys[SDL_SCANCODE_7]) gameState = HUB;
	if (keys[SDL_SCANCODE_Z]) cout << frameRate << endl;
}


void RenderScene(GLuint refShaderProgram) {
	// clear the screen
	glm::mat4 projection(1.0);
	projection = glm::perspective(float(60.0f*DEG_TO_RADIAN), 1920.0f / 1080.0f, 1.0f, 1000.0f);

	GLfloat scale(1.0f); // just to allow easy scaling of complete scene
	glm::mat4 modelview(1.0); // set base position for scene
	mvStack.push(modelview);

	Game_Camera.draw(mvStack.top(), character->getModelEye());
	rt3d::setUniformMatrix4fv(skyboxTest->shaderProgram, "projection", glm::value_ptr(projection));
	if (currentPass == 1)
		skyboxTest->draw(mvStack.top());

	/***********************************************************/

	// The light calculations for the shadows.
	glm::vec3 lightDirection = glm::vec3(light0.position[0], light0.position[1], light0.position[2]);
	glm::mat4 lightProjection = glm::ortho<float>(-200, 200, -200, 200, 0, 250);
	glm::mat4 lightViewPosition = glm::lookAt(lightDirection, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 lightView = lightProjection * lightViewPosition;
	glUseProgram(refShaderProgram);
	if (currentPass == 1)
		glUniformMatrix4fv(glGetUniformLocation(refShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	else
		glUniformMatrix4fv(glGetUniformLocation(refShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

	glUniformMatrix4fv(glGetUniformLocation(refShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(mvStack.top()));
	glUniform3fv(glGetUniformLocation(refShaderProgram, "viewPos"), 1, &Game_Camera.position[0]);
	glUniform3fv(glGetUniformLocation(refShaderProgram, "lightPos"), 1, &lightDirection[0]);
	glUniformMatrix4fv(glGetUniformLocation(refShaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightView));

	//glUniformMatrix4fv(glGetUniformLocation(program, uniformName), 1, GL_FALSE, data);
	//mvStack.pop();


	//Setting the light position for current shader.
	glm::vec4 tmp = mvStack.top()*lightPos;
	light0.position[0] = lightPos.x;
	light0.position[1] = lightPos.y;
	light0.position[2] = lightPos.z;
	rt3d::setMaterial(refShaderProgram, material1);
	rt3d::setLightPos(refShaderProgram, glm::value_ptr(tmp));
	rt3d::setLight(refShaderProgram, light0);


	mvStack.push(modelview);


	rt3d::setUniformMatrix4fv(houseTest->shaderProgram, "projection", glm::value_ptr(projection));
	//maze->baseShaderProgram = houseTest->shaderProgram;
	//

	//if(currentPass == 1)
	//houseTest->draw(mvStack.top(), refShaderProgram, currentPass);

	character->draw(mvStack.top(), refShaderProgram, currentPass);

	if (gameState == HUB)
	{
		if (currentPass == 1)
			terrain->draw(mvStack.top(), refShaderProgram, currentPass);
		for (int i = 0; i < Game_Hub_Characters.size(); i++)
		{
			Game_Hub_Characters[i]->draw(mvStack.top(), refShaderProgram, currentPass);
		}
		for (int i = 0; i < Game_Hub_Prefabs.size(); i++)
		{
			Game_Hub_Prefabs[i].draw(mvStack.top(), refShaderProgram, currentPass);
		}
		for (int i = 0; i < Game_Hub_Characters_Shop.size(); i++)
		{
			Game_Hub_Characters_Shop[i]->draw(mvStack.top());
		}

		//character->weapon->draw(mvStack.top(), character->position, character->currentAnimation, character->rotation, refShaderProgram, depthMap, currentPass);
		//testWeapon->draw(mvStack.top(), refShaderProgram, currentPass);

		if (openShop)
		{

			ui->textBox(text[2], -0.55, true, names[0]);
		}
		ui->statusBar(0.9, 0, (float)character->health / 200);
		ui->statusBar(0.8, 1, (float)character->manaPool / 200);

	}
	else if (gameState == COMBAT)
	{
		if (currentPass == 1)
		{
			terrain->draw(mvStack.top(), refShaderProgram, currentPass);
		}
	}
	else if (gameState == MAZE)
	{

		if (currentPass == 1)
			terrain->draw(mvStack.top(), refShaderProgram, currentPass);
		maze->draw(mvStack.top(), character, &Game_Camera, refShaderProgram, currentPass);

		ui->statusBar(0.9, 0, (float)character->health / 200);
		ui->statusBar(0.8, 1, (float)character->manaPool / 200);

		ui->QuestMarker(mvStack.top(), refShaderProgram, character, Game_Maze_Characters);

		/*for (int i = 0; i < Game_Maze_Characters.size(); i++)
		{
			if (Game_Maze_Characters[i]->health > 0)
				Game_Maze_Characters[i]->draw(mvStack.top(), refShaderProgram, currentPass);
		}*/

	}

	else if (gameState == MENU) {
		ui->mainMenu();
		if (ui->triggerHub() == true) {
			gameState = HUB;
		}
	}
	
	if (character->inCombat)
	{
		ui->statusBar(0.9, 0, (float)character->health / 200);
		ui->statusBar(0.8, 1, (float)character->manaPool / 200);
		ui->button();
	}
	
	//ui->textBox(text[0], -0.55, true, names[0]);
	//ui->textBox(text[1], -0.75, false, names[0]);


	//character->manaPool = 10;

	//ui->inventory(character);

	currentPass++;

	// remember to use at least one pop operation per push...
	mvStack.pop(); // initial matrix
	glDepthMask(GL_TRUE);

	//SDL_GL_SwapWindow(window); // swap buffers
}

/*Draw function - Renders the scene twice, first from the lights view and then from the game camera position. 
If you're using an object that isn't inside the predefined vector lists then you will need to manually update the 
gameobjects depthmap otherwise shadows will not work for it. - Scott */
void draw(SDL_Window * window)
{
	currentPass = 0;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, depthMapFBO);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	RenderScene(simpleDepthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Render to frame buffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, screenWidth, screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Passing in the depth map into the classes
	houseTest->SetDepthMap(depthMap);
	terrain->SetDepthMap(depthMap);
	//shadow_Debug->SetDepthMap(depthMap);
	character->SetDepthMap(depthMap);
	testWeapon->SetDepthMap(depthMap);

	if (gameState == HUB)
	{
		for (int i = 0; i < Game_Hub_Characters.size(); i++)
		{
			Game_Hub_Characters[i]->SetDepthMap(depthMap);
		}
		for (int i = 0; i < Game_Hub_Prefabs.size(); i++)
		{
			Game_Hub_Prefabs[i].SetDepthMap(depthMap);
		}
		for (int i = 0; i < Game_Hub_Characters_Shop.size(); i++)
		{
			Game_Hub_Characters_Shop[i]->SetDepthMap(depthMap);
		}
	}
	else if (gameState == COMBAT)
	{

	}
	else if (gameState == MAZE)
	{
		for (int i = 0; i < Game_Maze_Characters.size(); i++)
		{
			Game_Maze_Characters[i]->SetDepthMap(depthMap);
		}
	}



	RenderScene(normalShadowProgram);
	SDL_GL_SwapWindow(window); // swap buffers

}


// Program entry point - SDL manages the actual WinMain entry point for us
int main(int argc, char *argv[]) {
	SDL_Window * hWindow; // window handle
	SDL_GLContext glContext; // OpenGL context handle
	hWindow = setupRC(glContext); // Create window and render context 

								  // Required on Windows *only* init GLEW to access OpenGL beyond 1.1
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) { // glewInit failed, something is seriously wrong
		std::cout << "glewInit failed, aborting." << endl;
		exit(1);
	}
	cout << glGetString(GL_VERSION) << endl;

	init();

	bool running = true; // set running to true
	SDL_Event sdlEvent;  // variable to detect SDL events
	while (running) {	// the event loop
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT)
				running = false;
		}
		update();
		draw(hWindow); // call the draw function
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(hWindow);
	SDL_Quit();
	return 0;
}