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
#include <stack>
#include "md2model.h"
#include <SDL_ttf.h>

#include <vector>
#include <ctime>
#include <iostream>
#include <windows.h>
#include <conio.h>
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

glm::vec2 enemyMove;

GLfloat playerRotation = 0.0f;
GLfloat r = 0.0f;

glm::vec3 eye(0.0f, 1.0f, 0.0f);
glm::vec3 at(0.0f, 1.0f, -1.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);

stack<glm::mat4> mvStack;

// TEXTURE STUFF
GLuint textures[2];
GLuint skybox[5];
GLuint labels[5];

rt3d::lightStruct light0 = {
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // ambient
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
	{ -5.0f, 4.0f, 2.0f, 1.0f }  // position
};
glm::vec4 lightPos(8.0f, 5.0f, -12.0f, 0.0f); //light position

rt3d::materialStruct material0 = {
	{ 0.6f, 0.4f, 0.2f, 1.0f }, // ambient
	{ 0.5f, 1.0f, 0.5f, 1.0f }, // diffuse
	{ 0.0f, 0.1f, 0.0f, 1.0f }, // specular
	2.0f  // shininess
};
rt3d::materialStruct material1 = {
	{ 1.0f, 0.4f, 0.4f, 1.0f }, // ambient
	{ 1.0f, 0.8f, 1.0f, 1.0f }, // diffuse
	{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
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

const char *skyboxFiles[6] = {
	"red-sky/red_sky_front.bmp", "red-sky/red_sky_back.bmp", "red-sky/red_sky_right.bmp", "red-sky/red_sky_left.bmp", "red-sky/red_sky_top.bmp", "red-sky/red_sky_top.bmp"
};

//const char *skyboxFiles[6] = {
//	"Lomnia_End_Skybox/front.bmp", "Lomnia_End_Skybox/back.bmp", "Lomnia_End_Skybox/left.bmp", "Lomnia_End_Skybox/right.bmp", "Lomnia_End_Skybox/up.bmp", "Lomnia_End_Skybox/down.bmp"
//};


//Combat variables
bool inCombat = false;

glm::vec3 oldPlayerPos;

vector<Prefab> Game_Hub_Prefabs;
vector<Character> Game_Hub_Characters;
vector<Character> Game_Hub_Characters_Shop;

Camera Game_Camera = Camera();
Character* static_character[15];
PlayableCharacter* character;// = new PlayableCharacter();
Prefab* shadow_Debug;// = new Gameobject()
Skybox* skyboxTest;
MazeGenerator* maze;
Prefab* houseTest;// = new Prefab();
Terrain* terrain;// = new Terrain();

GLuint normalShadowProgram;
GLuint realShadowShader;
GLuint simpleDepthShader;
GLuint debugDepthQuad;
GLuint depthMapFBO = 0;
GLuint depthMap;
const GLuint SHADOW_WIDTH = 1920, SHADOW_HEIGHT = 1080;
int currentPass = 0;
GLuint screenHeight = 600;
GLuint screenWidth = 800;

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
		800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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

/*glm::vec3 getEnemyPos()
{
glm::vec3 enemyPos = glm::vec3(0, 0, 0);
int x, z;
do {
enemyPos.x = rand() % SIZE;
enemyPos.z = rand() % SIZE;
enemyPos.x = enemyPos.x * 3;
enemyPos.z = enemyPos.z * 3;
enemyPos.y = 0.8;
x = enemyPos.x;
z = enemyPos.z;
} while (Level[x/3][z/3].display != '_');
return enemyPos;
}


glm::vec2 moveEnemy()
{
glm::vec2 movePos;
//int x = enemyPos.x;
//int z = enemyPos.z;

//if (Level[x + 3][z].display == '_')
//	movePos = glm::vec2(0.1, 0);
//else if (Level[x - 3][z].display == '_')
//	movePos = glm::vec2(-0.1, 0);
//else if (Level[x][z + 3].display == '_')
//	movePos = glm::vec2(0, 0.1);
// else if (Level[x][z - 3].display == '_')
//	movePos = glm::vec2(0, -0.1);
// else {
int dir = rand() % 4;
if (dir == 0)
movePos = glm::vec2(0.1, 0);
if (dir == 1)
movePos = glm::vec2(-0.1, 0);
if (dir == 2)
movePos = glm::vec2(0, 0.1);
if (dir == 3)
movePos = glm::vec2(0, -0.1);


return movePos;
}

*/


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



	shaderProgram = rt3d::initShaders("phong-tex.vert", "phong-tex.frag");
	rt3d::setLight(shaderProgram, light0);
	rt3d::setMaterial(shaderProgram, material0);

	skyboxProgram = rt3d::initShaders("textured.vert", "textured.frag");

	normalShadowProgram = rt3d::initShaders("realTimeShadow.vert", "realTimeShadow.frag");
	glUniform1i(glGetUniformLocation(normalShadowProgram, "diffuseTexture"), 1);
	glUniform1i(glGetUniformLocation(normalShadowProgram, "shadowMap"), 0);
	simpleDepthShader = rt3d::initShaders("shadow.vert", "shadow.frag");


	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj("cube.obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	textures[0] = loadBitmap("lava_cube.bmp");
	meshObjects[0] = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());

	textures[1] = loadBitmap("hobgoblin2.bmp");
	meshObjects[1] = tmpModel.ReadMD2Model("tris.MD2");
	md2VertCount = tmpModel.getVertDataCount();

	meshObjects[2] = arnould.ReadMD2Model("arnould.MD2");
	md2VertCount2 = arnould.getVertDataCount();

	meshObjects[3] = weapon.ReadMD2Model("Partical_sword.MD2");
	md2VertCount3 = weapon.getVertDataCount();


	/*skybox[0] = loadBitmap("red-sky/Town_ft.bmp");
	skybox[1] = loadBitmap("red-sky/Town_bk.bmp");
	skybox[2] = loadBitmap("red-sky/Town_lf.bmp");
	skybox[3] = loadBitmap("Town-skybox/Town_rt.bmp");
	//skybox[4] = loadBitmap("Town-skybox/Town_up.bmp");*/

	skybox[0] = loadBitmap("red-sky/red_sky_front.bmp");
	skybox[1] = loadBitmap("red-sky/red_sky_back.bmp");
	skybox[3] = loadBitmap("red-sky/red_sky_left.bmp");
	skybox[2] = loadBitmap("red-sky/red_sky_right.bmp");
	skybox[4] = loadBitmap("red-sky/red_sky_top.bmp");


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// set up TrueType / SDL_ttf
	if (TTF_Init() == -1)
		cout << "TTF failed to initialise." << endl;

	textFont = TTF_OpenFont("MavenPro-Regular.ttf", 48);
	if (textFont == NULL)
		cout << "Failed to open font." << endl;

	labels[0] = 0;//First init to 0 to avoid memory leakage if it is dynamic
	labels[0] = textToTexture(" Hello ", labels[0]);//Actual set up of label. If dynamic, this should go in draw function



	//Initialize(Level);
	//GenerateMaze(Level, posX, posY, goalX, goalY);
	
	//playerPos = glm::vec3(posX, 0.8, posY);
	//enemyPos = getEnemyPos();

	//enemyMove = moveEnemy();

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
	maze = new MazeGenerator(shaderProgram);
	character = new PlayableCharacter("Arnold", "Models/arnould.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(0), shaderProgram);
	Game_Camera.InitalStats();
	character->InitalStats(shaderProgram);

	//Update
	//houseTest = new Prefab(shaderProgram, "Models/Shop_002.obj" /*"Models/desert.obj"*/ /*"Models/House_001.obj"*/, "Models/Textures/House_001.bmp",glm::vec3(1.3,1.3,1.3),glm::vec3(-10,-0.5,-10));
	//houseTest = new Prefab(shaderProgram, "Models/Shop_001.obj" /**/ /*"Models/House_001.obj"*/, "Models/Textures/Shop_001.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(0, -1, 0));
	//houseTest = new Prefab(shaderProgram, "Models/House_002.obj" /**/ /*"Models/House_001.obj"*/, "Models/Textures/House_002.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(0, -1, 0));
	//houseTest = new Prefab(shaderProgram, "Models/Shop_002.obj" /**/ /*"Models/House_001.obj"*/, "Models/Textures/Shop_002.bmp", glm::vec3(1.5, 1.5, 1.5), glm::vec3(0, -1, 0));
	//houseTest = new Prefab(shaderProgram, "Models/House_003.obj" /**/ /*"Models/House_001.obj"*/, "Models/Textures/House_003.bmp", glm::vec3(2.0, 2.0, 2.0), glm::vec3(0, -1, 0)); //Broken but could be used as a back prop out of the way.
	//houseTest = new Prefab(shaderProgram, "Models/Well.obj" /**/ /*"Models/House_001.obj"*/, "Models/Textures/Well.bmp", glm::vec3(2.0, 2.0, 2.0), glm::vec3(0, -1, 0)); //Broken but could be used as a back prop out of the way.
	//houseTest = new Prefab(shaderProgram, "Models/Teleporter_Stand.obj" /**/ /*"Models/House_001.obj"*/, "Models/Textures/Well.bmp", glm::vec3(2.0, 2.0, 2.0), glm::vec3(0, -1, 0)); //Broken but could be used as a back prop out of the way.
	houseTest = new Prefab(shaderProgram, "cube.obj", "Models/Textures/House_002.bmp", glm::vec3(20.0, 0.1, 20.0), glm::vec3(0, -1, 0));
	terrain = new Terrain(shaderProgram, "Models/Desert_Terrain_New_Low.obj", "Models/Textures/Terrain_Sand.bmp", glm::vec3(1, 1, 1), glm::vec3(300, 0, -300), 0);


	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Corner_Ruin.obj", "Models/Textures/Texture_Alien.bmp", glm::vec3(0.03, 0.02, 0.03), glm::vec3(-61.1, 0.49, -41.55), -110));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Corner_Ruin.obj", "Models/Textures/Texture_Alien.bmp", glm::vec3(0.03, 0.02, 0.03), glm::vec3(3.3, 0.49, 71.2), -110));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Corner_Ruin.obj", "Models/Textures/Texture_Alien.bmp", glm::vec3(0.03, 0.02, 0.03), glm::vec3(89.5, 0.49, 83.26), -173.1));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Corner_Ruin.obj", "Models/Textures/Texture_Alien.bmp", glm::vec3(0.03, 0.02, 0.03), glm::vec3(86.8, 0.49, -65.8), 173.14));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Corner_Ruin.obj", "Models/Textures/Texture_Alien.bmp", glm::vec3(0.03, 0.02, 0.03), glm::vec3(128.7, 2.67, -54.8), 173.14));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Corner_Ruin.obj", "Models/Textures/Texture_Alien.bmp", glm::vec3(0.03, 0.02, 0.03), glm::vec3(20.12, 0.49, -72.95), 17.14));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Corner_Ruin.obj", "Models/Textures/Texture_Alien.bmp", glm::vec3(0.03, 0.02, 0.03), glm::vec3(22.43, 0.49, -59.3), 97.2));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_Player.obj", "Models/Textures/Texture_Alien.bmp", glm::vec3(2.5, 2, 2.5), glm::vec3(47.29, 2.54, 82.97), -123.7));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_Player.obj", "Models/Textures/Texture_Alien.bmp", glm::vec3(2.5, 2, 2.5), glm::vec3(-15.6, 2.54, -87.2), -123.7));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Shop_001.obj", "Models/Textures/Shop_001.bmp", glm::vec3(0.7, 0.7, 0.7), glm::vec3(-49.1, 0, -46.89), -99));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Shop_001.obj", "Models/Textures/Shop_001.bmp", glm::vec3(0.7, 0.7, 0.7), glm::vec3(19.1, 0, 70.05), 0));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Shop_002.obj", "Models/Textures/Shop_001.bmp", glm::vec3(1.3, 1.3, 1.3), glm::vec3(104.1, 0, -9.9), -90));


	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Well.obj", "Models/Textures/Well.bmp", glm::vec3(2.0, 2.0, 2.0), glm::vec3(61.1, 1.25, 0), 30));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Teleporter.obj", "Models/Textures/Well.bmp", glm::vec3(0.4, 0.4, 0.4), glm::vec3(39, 0.36, 0), 0));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Teleporter_Stand.obj", "Models/Textures/Well.bmp", glm::vec3(0.4, 0.4, 0.4), glm::vec3(39.33, 0.01, 2.52), 0));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Teleporter_Stand.obj", "Models/Textures/Well.bmp", glm::vec3(0.4, 0.4, 0.4), glm::vec3(34.6, 0.01, -2.1), 0));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Teleporter_Stand.obj", "Models/Textures/Well.bmp", glm::vec3(5, 5, 5), glm::vec3(-149.33, 1.86, 77.7), 0));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Teleporter_Stand.obj", "Models/Textures/Well.bmp", glm::vec3(10, 10, 10), glm::vec3(15.5, 1.86, -126), 0));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_001.obj", "Models/Textures/Well.bmp", glm::vec3(0.02, 0.02, 0.02), glm::vec3(-42.3, 0, -18.1), -22));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_001.obj", "Models/Textures/Well.bmp", glm::vec3(0.02, 0.02, 0.02), glm::vec3(-22.16, 0, -39.8), -49));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_001.obj", "Models/Textures/Well.bmp", glm::vec3(0.02, 0.02, 0.02), glm::vec3(52.4, 0, -48.6), 23.4));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_001.obj", "Models/Textures/Well.bmp", glm::vec3(0.02, 0.02, 0.02), glm::vec3(54.5, 0, 43.9), -28.2));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_001.obj", "Models/Textures/Well.bmp", glm::vec3(0.02, 0.02, 0.02), glm::vec3(0, 0, 54.1), 19.4));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_001.obj", "Models/Textures/Well.bmp", glm::vec3(0.02, 0.02, 0.02), glm::vec3(121.1, 0, -28.1), 23.4));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Destroyed_House_001.obj", "Models/Textures/Rusty_Shed_Metal_Texture.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(151, -2.51, -22), -149));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Destroyed_House_001.obj", "Models/Textures/Rusty_Shed_Metal_Texture.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(163, -0.64, -4.3), -72));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Destroyed_House_001.obj", "Models/Textures/Rusty_Shed_Metal_Texture.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(-51.9, 0, -75), -83));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Destroyed_House_001.obj", "Models/Textures/Rusty_Shed_Metal_Texture.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(67.6, 0, -69.4), -72));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Destroyed_House_001.obj", "Models/Textures/Rusty_Shed_Metal_Texture.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(110, 0, 20), -30.4));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Destroyed_House_001.obj", "Models/Textures/Rusty_Shed_Metal_Texture.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(21.4, 0, 96.3), 0));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Destroyed_House_001.obj", "Models/Textures/Rusty_Shed_Metal_Texture.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(0, 0, 0), -47.9));

	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/Texture1.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(17, 0, -88), -205));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/Texture1.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(144.5, 1.07, 17.43), 39.4));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/Texture1.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(-68.3, 0, -18), -93.5));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/Texture1.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(62.9, 0, 61.5), -64.4));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/Texture1.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(86.5, 0, 58.8), 39.4));
	Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/Texture1.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(97.11, 0, -50.2), 150));

	//Buildings etc.. non useful or usable items
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Teleporter.obj", "Models/Textures/Texture_Alien.bmp", glm::vec3(0.4, 0.4, 0.4), glm::vec3(0, -1, 0), -80));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Teleporter_Stand.obj", "Models/Textures/Metal_Alien_Texture.bmp", glm::vec3(0.4, 0.4, 0.4), glm::vec3(2, -2, -0)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Teleporter_Stand.obj", "Models/Textures/Metal_Alien_Texture.bmp", glm::vec3(0.4, 0.4, 0.4), glm::vec3(-6, -2, -0)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Well.obj", "Models/Textures/Well.bmp", glm::vec3(2.0, 2.0, 2.0), glm::vec3(6, -1, 8)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Shop_001.obj", "Models/Textures/Shop_001.bmp", glm::vec3(0.7, 0.7, 0.7), glm::vec3(50, -1, -60)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Shop_002.obj", "Models/Textures/Shop_002.bmp", glm::vec3(1.5, 1.5, 1.5), glm::vec3(20, -1, 80)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Shop_002.obj", "Models/Textures/Shop_002.bmp", glm::vec3(1.5, 1.5, 1.5), glm::vec3(-60, -1, -20)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_Player.obj", "Models/Textures/House_001.bmp", glm::vec3(2.5, 2.0, 2.5), glm::vec3(30, 1, 40)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_001.obj", "Models/Textures/House_001.bmp", glm::vec3(0.02, 0.02, 0.02), glm::vec3(-60, -1, -50)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/House_002.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(-0, -3, 40)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_003.obj", "Models/Textures/House_003.bmp", glm::vec3(2.0, 2.0, 2.0), glm::vec3(-30, -1.5, -40)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Destroyed_House_001.obj", "Models/Textures/Rusty_Shed_Metal_Texture.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(40, -3, 0)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Destroyed_House_001.obj", "Models/Textures/Rusty_Shed_Metal_Texture.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(80, -3, 20)));

	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_001.obj", "Models/Textures/House_001.bmp", glm::vec3(0.02, 0.02, 0.02), glm::vec3(0, -1.5, -80)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_001.obj", "Models/Textures/House_002.bmp", glm::vec3(0.02, 0.02, 0.02), glm::vec3(50, -1.5, 30)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_001.obj", "Models/Textures/House_003.bmp", glm::vec3(0.02, 0.02, 0.02), glm::vec3(20, -1.5, -50)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_001.obj", "Models/Textures/Well.bmp", glm::vec3(0.02, 0.02, 0.02), glm::vec3(10, -1.5, 70)));

	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/Texture1.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(-30, -3, 60)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/House_002.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(-70, -3, -10)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/House_002.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(0, -3, 35)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/House_001.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(-20, -3, 40)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/House_001.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(-20, -3, -40)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/House_001.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(80, -3, 100)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/House_002.obj", "Models/Textures/House_002.bmp", glm::vec3(60.0, 60.0, 60.0), glm::vec3(-80, -3, -50)));

	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Destroyed_House_001.obj", "Models/Textures/House_002.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(-80, -3, -20)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Destroyed_House_001.obj", "Models/Textures/Techy_Metal.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(-50, -3, 20)));
	//Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Destroyed_House_001.obj", "Models/Textures/House_002.bmp", glm::vec3(1.0, 1.0, 1.0), glm::vec3(-25, -3, -80)));

	/*Game_Hub_Prefabs.push_back(Prefab(shaderProgram, "Models/Loot_Drop_001.obj", "Models/Textures/House_002.bmp", glm::vec3(0.01, 0.01, 0.01), glm::vec3(6, -1, 8)));*/



	//Game_Hub_Characters.push_back(Character("Arnold", "Models/ripper.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(50, 0, -30), shaderProgram));
	//Game_Hub_Characters.push_back(Character("Arnold", "Models/quigon.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(30, 0, 20), shaderProgram));
	//Game_Hub_Characters.push_back(Character("Arnold", "Models/pogo_bunny.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(10, 0, 10), shaderProgram));


	//NPCs in the hub area
	//Game_Hub_Characters.push_back(Character("Arnold", "Models/arnould.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(1, 0, 0), shaderProgram));

	//NPCS that serve a purpose (shop owners/traders) 
	//Game_Hub_Characters_Shop.push_back(Character("Arnold", "Models/arnould.MD2", "hobgoblin2.bmp", glm::vec3(1), glm::vec3(-4, 2, -4), shaderProgram));

	lightPos.x += 50;
	lightPos.y += 100;
	lightPos.z += 50;
	//shadow_Debug = new Prefab(shaderProgram, "Models/House_Player.obj", "Models/Textures/House_002.bmp", glm::vec3(500.0, 0.1, 500.0), glm::vec3(0, -5.0, 0), 0);

}

glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d) {
	return glm::vec3(pos.x + d*std::sin(r*DEG_TO_RADIAN), pos.y, pos.z - d*std::cos(r*DEG_TO_RADIAN));
}

glm::vec3 moveRight(glm::vec3 pos, GLfloat angle, GLfloat d) {
	return glm::vec3(pos.x + d*std::cos(r*DEG_TO_RADIAN), pos.y, pos.z + d*std::sin(r*DEG_TO_RADIAN));
}

void update(void) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	Game_Camera.update(character->getModelEye(), character->getRotation());
	character->Update();
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
	glm::mat4 lightProjection = glm::ortho<float>(-60, 60, -60, 60, 0, 250);
	glm::mat4 lightViewPosition = glm::lookAt(lightDirection, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 lightView = lightProjection * lightViewPosition;
	glUseProgram(refShaderProgram);
	cout << currentPass << endl;


	glUseProgram(refShaderProgram);


	glUniformMatrix4fv(glGetUniformLocation(refShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
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
	maze->baseShaderProgram = houseTest->shaderProgram;
	maze->draw(mvStack.top());

	character->draw(mvStack.top(), refShaderProgram, currentPass);

	if (currentPass == 1)
		terrain->draw(mvStack.top(), refShaderProgram, currentPass);

	for (int i = 0; i < Game_Hub_Characters.size(); i++)
	{
		Game_Hub_Characters[i].draw(mvStack.top());
	}
	for (int i = 0; i < Game_Hub_Prefabs.size(); i++)
	{
		Game_Hub_Prefabs[i].draw(mvStack.top(), refShaderProgram, currentPass);
	}
	for (int i = 0; i < Game_Hub_Characters_Shop.size(); i++)
	{
		Game_Hub_Characters_Shop[i].draw(mvStack.top());
	}

	currentPass++;

	// remember to use at least one pop operation per push...
	mvStack.pop(); // initial matrix
	glDepthMask(GL_TRUE);

	//SDL_GL_SwapWindow(window); // swap buffers
}


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

	cout << " x: " << Game_Camera.position.x << " y: " << Game_Camera.position.y << " z: " << Game_Camera.position.z << endl;

	//Passing in the depth map into the classes
	houseTest->SetDepthMap(depthMap);
	terrain->SetDepthMap(depthMap);
	//shadow_Debug->SetDepthMap(depthMap);
	character->SetDepthMap(depthMap);
	//cout << lightPos.x << " y: " << lightPos.y << " light pos: \z: " << lightPos.z << endl;
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