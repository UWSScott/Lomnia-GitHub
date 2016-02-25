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

#include <ctime>
#include <iostream>
#include <ctime>
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
	{1.0f, 0.3f, 0.3f, 1.0f}, // ambient
	{1.0f, 1.0f, 1.0f, 1.0f}, // diffuse
	{1.0f, 1.0f, 1.0f, 1.0f}, // specular
	{-10.0f, 10.0f, 10.0f, 1.0f}  // position
};
glm::vec4 lightPos(-10.0f, 10.0f, 10.0f, 1.0f); //light position

rt3d::materialStruct material0 = {
	{0.6f, 0.4f, 0.2f, 1.0f}, // ambient
	{0.5f, 1.0f, 0.5f, 1.0f}, // diffuse
	{0.0f, 0.1f, 0.0f, 1.0f}, // specular
	2.0f  // shininess
};
rt3d::materialStruct material1 = {
	{1.0f, 0.4f, 0.4f, 1.0f}, // ambient
	{1.0f, 0.8f, 1.0f, 1.0f}, // diffuse
	{1.0f, 0.8f, 0.8f, 1.0f}, // specular
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

#define SIZE 17
// CELL STRUCTURE
struct Cell
{
	bool visited;
	bool top_wall;
	bool bot_wall;
	bool left_wall;
	bool right_wall;
	char display;
};

// FUNCTION DECLARATIONS
void Initialize(Cell Level[][SIZE]);
//void ClearScreen();
//void Redraw(Cell Level[][SIZE]);
void GenerateMaze(Cell Level[][SIZE], int &posX, int &posY, int &goalX, int &goalY);
void SaveMaze();
void LoadMaze();




Cell Level[SIZE][SIZE];
int posX = 0;
int posY = 0;
int goalX = 0;
int goalY = 0;
int totalCells = ((SIZE - 1) / 2)*((SIZE - 1) / 2);

//Combat variables
//Character player = Character("PLAYER", 100, 100, 10, 10, 1.0, 1.0, 1.0, 1.0, true);
//Character enemy = Character("ENEMY"  , 25, 0, 14, 10, 1.0, 2.0, 0.5, 1.0, false);
//Character player = new Character();
bool inCombat = false;
std::clock_t start;
double duration;
glm::vec3 oldPlayerPos;

Camera Game_Camera = Camera();
//Character character = Character();
PlayableCharacter* character = new PlayableCharacter();
Weapon weaponTest = Weapon();// "Scott's Saber", "Partical_sword.MD2", "hobgoblin2.bmp", 0, 5, 5, "SWORD", 1, shaderProgram);
Skybox skyboxTest = Skybox();

const char *skyboxFiles[6] = {
	"red-sky/red_sky_front.bmp", "red-sky/red_sky_back.bmp", "red-sky/red_sky_right.bmp", "red-sky/red_sky_left.bmp", "red-sky/red_sky_top.bmp", "red-sky/red_sky_top.bmp"
};

// textToTexture
GLuint textToTexture(const char * str, GLuint textID/*, TTF_Font *font, SDL_Color colour, GLuint &w,GLuint &h */) {
	TTF_Font *font = textFont;
	SDL_Color colour = { 255, 255, 255 };
	SDL_Color bg = { 0, 0, 0 };

	SDL_Surface *stringImage;
	stringImage = TTF_RenderText_Blended(font,str,colour);

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
	} else {             // no alpha
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

// INITIALIZE MAZE
void Initialize(Cell Level[][SIZE]) {
	for (int i = 0; i<SIZE; i++) {
		for (int j = 0; j<SIZE; j++) {
			Level[i][j].display = '*';
			Level[i][j].visited = false;
			Level[i][j].top_wall = true;
			Level[i][j].bot_wall = true;
			Level[i][j].left_wall = true;
			Level[i][j].right_wall = true;
		}
	}
	for (int i = 1; i<SIZE - 1; i++) {
		for (int j = 1; j<SIZE - 1; j++) {
			// Border Cells have fewer accessible walls
			Level[1][j].top_wall = false;
			Level[SIZE - 2][j].bot_wall = false;
			Level[i][1].left_wall = false;
			Level[i][SIZE - 2].right_wall = false;
		}
	}
}

// GENERATE MAZE
void GenerateMaze(Cell Level[][SIZE], int &posX, int &posY, int &goalX, int &goalY) {
	srand((unsigned)time(NULL));                                                                            // Pick random start cell
	int random = 0;
	int randomX = ((2 * rand()) + 1) % (SIZE - 1);                                          // Generate a random odd number between 1 and SIZE
	int randomY = ((2 * rand()) + 1) % (SIZE - 1);                                          // Generate a random odd number between 1 and SIZE
	posX = randomX *3;                                                                 // Save player's initial X position
	posY = randomY * 3;                                                                 // Save player's initial Y position
	int visitedCells = 1;
	//int totalCells = ((SIZE - 1) / 2)*((SIZE - 1) / 2);
	int percent = 0;
	stack<int> back_trackX, back_trackY;                                            // Stack is used to trace the reverse path

	Level[randomY][randomX].display = '_';                                          // Set S as the start cell
	Level[randomY][randomX].visited = true;                                         // Set start cell as visited;

	while (visitedCells < totalCells)
	{
		if (((Level[randomY - 2][randomX].visited == false) && (Level[randomY][randomX].top_wall == true && Level[randomY - 2][randomX].bot_wall == true)) ||
			((Level[randomY + 2][randomX].visited == false) && (Level[randomY][randomX].bot_wall == true && Level[randomY + 2][randomX].top_wall == true)) ||
			((Level[randomY][randomX - 2].visited == false) && (Level[randomY][randomX].left_wall == true && Level[randomY][randomX - 2].right_wall == true)) ||
			((Level[randomY][randomX + 2].visited == false) && (Level[randomY][randomX].right_wall == true && Level[randomY][randomX + 2].left_wall == true)))
		{
			random = (rand() % 4) + 1;              // Pick a random wall 1-4 to knock down

													// GO UP
			if ((random == 1) && (randomY > 1)) {
				if (Level[randomY - 2][randomX].visited == false) {        // If not visited
					Level[randomY - 1][randomX].display = '_';        // Delete display
					Level[randomY - 1][randomX].visited = true;       // Mark cell as visited
					Level[randomY][randomX].top_wall = false;       // Knock down wall

					back_trackX.push(randomX);                      // Push X for back track
					back_trackY.push(randomY);                      // Push Y for back track

					randomY -= 2;                                   // Move to next cell
					Level[randomY][randomX].visited = true;         // Mark cell moved to as visited
					Level[randomY][randomX].display = '_';          // Update path
					Level[randomY][randomX].bot_wall = false;       // Knock down wall
					visitedCells++;                                 // Increase visitedCells counter
				}
				else
					continue;
			}

			// GO DOWN
			else if ((random == 2) && (randomY < SIZE - 2)) {
				if (Level[randomY + 2][randomX].visited == false) {        // If not visited
					Level[randomY + 1][randomX].display = '_';        // Delete display
					Level[randomY + 1][randomX].visited = true;       // Mark cell as visited
					Level[randomY][randomX].bot_wall = false;       // Knock down wall

					back_trackX.push(randomX);                      // Push X for back track
					back_trackY.push(randomY);                      // Push Y for back track

					randomY += 2;                                   // Move to next cell
					Level[randomY][randomX].visited = true;         // Mark cell moved to as visited
					Level[randomY][randomX].display = '_';          // Update path
					Level[randomY][randomX].top_wall = false;       // Knock down wall
					visitedCells++;                                 // Increase visitedCells counter
				}
				else
					continue;
			}

			// GO LEFT
			else if ((random == 3) && (randomX > 1)) {
				if (Level[randomY][randomX - 2].visited == false) {        // If not visited
					Level[randomY][randomX - 1].display = '_';        // Delete display
					Level[randomY][randomX - 1].visited = true;       // Mark cell as visited
					Level[randomY][randomX].left_wall = false;      // Knock down wall

					back_trackX.push(randomX);                      // Push X for back track
					back_trackY.push(randomY);                      // Push Y for back track

					randomX -= 2;                                   // Move to next cell
					Level[randomY][randomX].visited = true;         // Mark cell moved to as visited
					Level[randomY][randomX].display = '_';          // Update path
					Level[randomY][randomX].right_wall = false;     // Knock down wall
					visitedCells++;                                 // Increase visitedCells counter
				}
				else
					continue;
			}

			// GO RIGHT
			else if ((random == 4) && (randomX < SIZE - 2)) {
				if (Level[randomY][randomX + 2].visited == false) {        // If not visited
					Level[randomY][randomX + 1].display = '_';        // Delete display
					Level[randomY][randomX + 1].visited = true;       // Mark cell as visited
					Level[randomY][randomX].right_wall = false;     // Knock down wall

					back_trackX.push(randomX);                      // Push X for back track
					back_trackY.push(randomY);                      // Push Y for back track

					randomX += 2;                                   // Move to next cell
					Level[randomY][randomX].visited = true;         // Mark cell moved to as visited
					Level[randomY][randomX].display = '_';          // Update path
					Level[randomY][randomX].left_wall = false;      // Knock down wall
					visitedCells++;                                 // Increase visitedCells counter
				}
				else
					continue;
			}

			percent = (visitedCells * 100 / totalCells * 100) / 100;                // Progress in percentage
			cout << endl << "       Generating a Random Maze... " << percent << "%" << endl;
		}
		else {
			randomX = back_trackX.top();
			back_trackX.pop();

			randomY = back_trackY.top();
			back_trackY.pop();
		}

		//ClearScreen();
		//Redraw(Level);
	}

	goalX = randomX;
	goalY = randomY;
	Level[goalY][goalX].display = '.';
	//system("cls");
	//ClearScreen();
	//Redraw(Level);
	//cout << endl << "\a\t   Complete!" << endl;
}

// SAVE MAZE -- Cell Level[][SIZE]
void SaveMaze() {
	ofstream output;
	char file[20] = "MazeSeed";
	char input;
	int passes = 0;
	cout << endl << "Saving Maze....";
	//cin >> input;

	/*if ((input == 'y') || (input == 'Y')) {
		cout << endl << "Save as: ";
		cin >> file;*/

	output.open(file);

	for (int i = 0; i < SIZE; i++) {
		output << endl;
		for (int j = 0; j < SIZE; j++) {
			passes++;
			output << Level[i][j].display;
		}
	}

	cout << endl << "... Complete!";
	cout << "Maze has been saved to" << "\"" << file << "\"" << endl;
	cout << passes << endl;
	output.close();
}

void LoadMaze()
{
	char space;
	ifstream myfile("MazeSeed");

	if (myfile.is_open()) {
		for (int row = 0; row < SIZE; row++) {
			for (int col = 0; col < SIZE; col++) {
				myfile >> space;
				cout << space;
				Level[row][col].display = space;
				cout << space;
			}
			cout << endl;
		}
		cout << "Maze Loaded!" << endl;
	} else { cout << "Unable to open file"; }
}

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
        800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
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
		std::cout << "Error loading bitmap" << std::endl;
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
		externalFormat = (format->Rmask < format-> Bmask) ? GL_RGBA : GL_BGRA;
	}
	else {
		internalFormat = GL_RGB;
		externalFormat = (format->Rmask < format-> Bmask) ? GL_RGB : GL_BGR;
	}

	glTexImage2D(GL_TEXTURE_2D,0,internalFormat,tmpSurface->w, tmpSurface->h, 0,
		externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(tmpSurface); // texture loaded, free the temporary buffer
	return texID;	// return value of texture ID
}

glm::vec3 getEnemyPos()
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




void init(void) {
	start = std::clock();


	shaderProgram = rt3d::initShaders("phong-tex.vert","phong-tex.frag");
	rt3d::setLight(shaderProgram, light0);
	rt3d::setMaterial(shaderProgram, material0);

	skyboxProgram = rt3d::initShaders("textured.vert","textured.frag");

	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj("cube.obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	textures[0] = loadBitmap("lava_cube.bmp");
	meshObjects[0] = rt3d::createMesh(verts.size()/3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());

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
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


	// set up TrueType / SDL_ttf
	if (TTF_Init()== -1)
		cout << "TTF failed to initialise." << endl;

	textFont = TTF_OpenFont("MavenPro-Regular.ttf", 48);
	if (textFont == NULL)
		cout << "Failed to open font." << endl;
		
	labels[0] = 0;//First init to 0 to avoid memory leakage if it is dynamic
	labels[0] = textToTexture(" Hello ", labels[0]);//Actual set up of label. If dynamic, this should go in draw function



	Initialize(Level);
	GenerateMaze(Level, posX, posY, goalX, goalY);

	playerPos = glm::vec3(posX, 0.8, posY);
	enemyPos = getEnemyPos();

	enemyMove = moveEnemy();

	character = new PlayableCharacter("Arnold", 10, 10);
	Game_Camera.InitalStats();
	character->InitalStats(shaderProgram);
	skyboxTest.InitalStats(skyboxFiles);
	weaponTest.InitalStats(shaderProgram);
	
}

/*bool Collision(Collisions circle, Collisions circle2) {
	GLfloat radius = circle.getRadius();
	GLfloat x = circle.getX();
	GLfloat z = circle.getZ();

	GLfloat radius2 = circle2.getRadius();
	GLfloat x2 = circle2.getX();
	GLfloat z2 = circle2.getZ();

	int minDist = radius + radius2;
	int distance = sqrt(((x - x2) * (x - x2)) + ((z - z2) *(z - z2)));
	if (minDist >= distance) {
		return true;
	}
	else if ((minDist < distance))
	{
		return false;
	}

}*/

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

	/*if (inCombat == false)
	{
		enemyAnim = 1;
		if (keys[SDL_SCANCODE_W])
		{
			
			currentAnim = 1;

		}
		else if (keys[SDL_SCANCODE_S]) {
		
			currentAnim = 13;

		}
		else if (keys[SDL_SCANCODE_A]) {
			r -= 1.3f;
			currentAnim = 0;


		}
		else if (keys[SDL_SCANCODE_D]) {
			r += 1.3f;
			currentAnim = 0;

		}

		else if (keys[SDL_SCANCODE_SPACE])
		{
			currentAnim = 6;
		}
		else currentAnim = 0;
	} else {
		currentAnim = 14;
		enemyAnim = 10;
	}*/

	/*if (player.health > 0)
	{
		if (keys[SDL_SCANCODE_W]) { if(inCombat == false) playerPos = moveForward(playerPos, r, 0.1f);  eye = moveForward(eye, r, 0.1f); }
		if (keys[SDL_SCANCODE_S]) { if(inCombat == false) playerPos = moveForward(playerPos, -r - 180, -0.1f); eye = moveForward(eye, r, -0.1f); }
		if (keys[SDL_SCANCODE_A]) eye = moveRight(eye, r, -0.1f);
		if (keys[SDL_SCANCODE_D]) eye = moveRight(eye, r, 0.1f);
		if (keys[SDL_SCANCODE_R]) eye.y += 0.1;
		if (keys[SDL_SCANCODE_F]) eye.y -= 0.1;

		if (keys[SDL_SCANCODE_C] || inCombat == true) {
			camera = 1;
			heightOfCam = 1;
		}
		if (keys[SDL_SCANCODE_X]) {
			camera = 2;
			heightOfCam+=0.1f;
		}
		if (keys[SDL_SCANCODE_COMMA]) r -= 1.0f;
		if (keys[SDL_SCANCODE_PERIOD]) r += 1.0f;
		if (inCombat == false)
			playerRotation = r;
		if (keys[SDL_SCANCODE_B]) { Initialize(Level); GenerateMaze(Level, posX, posY, goalX, goalY); }
		if (keys[SDL_SCANCODE_M]) SaveMaze();
		if (keys[SDL_SCANCODE_N]) LoadMaze();

		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

		/*NOTE - Move combat to within the player class in final version. - Scott*/

		/*I need someone to block the input so it's not spammed when a button is pressed.
		Someone said they had this working for something else? */
	/*	if (keys[SDL_SCANCODE_0]) { if (inCombat == false) { cout << "COMBAT BEGINS!" << endl; inCombat = true; } }
		if (pressedButton == false)
		{
			if (keys[SDL_SCANCODE_1]) player.queuedAttacks.push_back(LightAttack());
			if (keys[SDL_SCANCODE_2]) player.queuedAttacks.push_back(HeavyAttack());
			if (keys[SDL_SCANCODE_3]) player.queuedAttacks.push_back(Poison());
			if (keys[SDL_SCANCODE_4]) player.queuedAttacks.push_back(Stun());
			if (keys[SDL_SCANCODE_5]) player.queuedAttacks.push_back(ItemUse(Item()));
			if (keys[SDL_SCANCODE_6]) player.queuedAttacks.push_back(Flee());
			pressedButton = true;
		} else {
			pressedButton = false;
		}

		if (keys[SDL_SCANCODE_Q])
		{
			for each (C_Attack var in player.queuedAttacks)
			{
				cout << var.GetAttackName() << " " << var.GetManaCost() << endl;
			}
			cout << "END " << player.queuedAttacks.size() << endl;
		}

		if (inCombat)
		{
			player.inCombat = true;
			enemy.inCombat = true;
			player.Attack(enemy);
			enemy.Attack(player);
			if (player.health <= 0 || enemy.health <= 0)
			{
				inCombat = false;
				player.inCombat = false;
				enemy.inCombat = false;
			}
		}
	} else {
		currentAnim = 5;
		enemyAnim = 7;
	}*/

	//player.Update((float)duration);
	//enemy.Update((float)duration);

	/*Collisions playerCircle = playerCircle.CollisionCircles(playerPos.x, playerPos.z, 1.0f);
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			
			if (Level[i][j].display == '*')
			{
				Collisions Circle = Circle.CollisionCircles(i * 3, j * 3, 0.9f);
				if (Collision(Circle, playerCircle) == true) {
					playerPos = oldPlayerPos;
				}
			}
		}
	}*/

	//oldPlayerPos = playerPos;

	/*Movement should be handled inside the enemy class for the full game.
	Create checks before moving to ensure the enemy isn't dead/incombat or idle etc.
	-Scott */

	/*For collision detection we want to pass in the object.id or name which we assign in the 'GameObject' class.
	Not sure if we've created it for the prototype, but for the finished product having the id means we can work 
	out what we need to do for that instance of collison. (ie, pickup weapon, start combat, stop movement etc.)
	-Scott */

	//if(inCombat == false && enemy.health > 0)
		//enemyPos = glm::vec3(enemyPos.x + enemyMove.x, enemyPos.y, enemyPos.z + enemyMove.y);

	/*Collisions enemyCircle = enemyCircle.CollisionCircles(enemyPos.x, enemyPos.z,1.0f);
	if (inCombat == false && Collision(playerCircle, enemyCircle) == true && player.health > 0 && enemy.health > 0) {
		cout << "PLAYER SPOTTED! -- BEGINING COMBAT!" << endl;
		inCombat = true;
		return;
	}*/

	/*for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {

			if (Level[i][j].display == '*')
			{
				Collisions Circle = Circle.CollisionCircles(i * 3, j * 3, 0.9f);
				if (Collision(Circle, enemyCircle) == true)
				{
					//cout << "boom" << endl;
					//enemyMove = moveEnemy();
					enemyMove = -enemyMove;
				}
			}
		}
	}*/
}




void draw(SDL_Window * window) {
	// clear the screen
	glEnable(GL_CULL_FACE);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 projection(1.0);
	projection = glm::perspective(float(60.0f*DEG_TO_RADIAN), 1920.0f / 1080.0f, 1.0f, 150.0f);
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projection));


	GLfloat scale(1.0f); // just to allow easy scaling of complete scene

	glm::mat4 modelview(1.0); // set base position for scene
	mvStack.push(modelview);

	/*if (camera == 1) {
		at = moveForward(eye, r, 1.0f);
		mvStack.top() = glm::lookAt(eye, at, up);
	}
	
	if (camera == 2){
	at = glm::vec3(playerPos.x, playerPos.y + heightOfCam, playerPos.z);
		eye = moveForward(at, r, -5.0f);
		mvStack.top() = glm::lookAt(eye, at, up);
	}*/
	//mvStack.top() = glm::lookAt(Game_Camera.eye, Game_Camera.at, Game_Camera.up);
	Game_Camera.draw(mvStack.top(), character->getModelEye());

	// draw a skybox
//	glUseProgram(skyboxProgram);
	rt3d::setUniformMatrix4fv(skyboxTest.shaderProgram, "projection", glm::value_ptr(projection));
	skyboxTest.draw(mvStack.top());
	//glDepthMask(GL_FALSE); // make sure depth test is off
	//glm::mat3 mvRotOnlyMat3 = glm::mat3(mvStack.top());
	//mvStack.push(glm::mat4(mvRotOnlyMat3));

	


	/*
	// front
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skybox[0]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 0.0f, -2.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	mvStack.pop();

	// back
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skybox[1]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 0.0f, 2.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	mvStack.pop();

	// left
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skybox[2]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-2.0f, 0.0f, 0.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	mvStack.pop();

	// right
	mvStack.push(mvStack.top());
	glBindTexture(GL_TEXTURE_2D, skybox[3]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(2.0f, 0.0f, 0.0f));
	rt3d::setUniformMatrix4fv(skyboxProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	mvStack.pop();

	mvStack.pop();

	// back to remainder of rendering
	glDepthMask(GL_TRUE); // make sure depth test is on
	*/

	glUseProgram(shaderProgram);

	glm::vec4 tmp = mvStack.top()*lightPos;
	light0.position[0] = tmp.x;
	light0.position[1] = tmp.y;
	light0.position[2] = tmp.z;
	rt3d::setLightPos(shaderProgram, glm::value_ptr(tmp));


	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projection));

	for (int i = 0; i<SIZE; i++) {
		for (int j = 0; j<SIZE; j++) {
			glBindTexture(GL_TEXTURE_2D, textures[0]);
			mvStack.push(mvStack.top());
			if (Level[i][j].display == '*')
			{
				mvStack.top() = glm::translate(mvStack.top(), glm::vec3(i * 3, 1.0f, j * 3));
				mvStack.top() = glm::scale(mvStack.top(), glm::vec3(3.0f, 2.0f, 3.0f));
			}
			else {

				mvStack.top() = glm::translate(mvStack.top(), glm::vec3(i  * 3, 0.0f, j  * 3));
				mvStack.top() = glm::scale(mvStack.top(), glm::vec3(3.0f, 0.1f, 3.0f));
			}
			mvStack.top() = glm::scale(mvStack.top(), glm::vec3(0.5f, 0.5f, 0.5f));
			rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
			rt3d::setMaterial(shaderProgram, material0);
			rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
			mvStack.pop();
		}
	}


	// Animate the md2 model, and update the mesh with new vertex data
	arnould.Animate(currentAnim, 0.1);
	rt3d::updateMesh(meshObjects[2], RT3D_VERTEX, arnould.getAnimVerts(), arnould.getVertDataSize());

	//draw the arnould
	glCullFace(GL_FRONT); // md2 faces are defined clockwise, so cull front face
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	rt3d::materialStruct tmpMaterial = material1;
	rt3d::setMaterial(shaderProgram, tmpMaterial);
	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), playerPos);
	mvStack.top() = glm::rotate(mvStack.top(), float(90.0f*DEG_TO_RADIAN), glm::vec3(-1.0f, 0.0f, 0.0f));
	mvStack.top() = glm::rotate(mvStack.top(), float(90.0f*DEG_TO_RADIAN - playerRotation / 57.5), glm::vec3(0.0f, 0.0f, 1.0f));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(scale*0.03, scale*0.03, scale*0.03));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawMesh(meshObjects[2], md2VertCount2, GL_TRIANGLES);
	mvStack.pop();
	glCullFace(GL_BACK);


	// Animate the md2 model, and update the mesh with new vertex data
	weapon.Animate(currentAnim, 0.1);
	rt3d::updateMesh(meshObjects[3], RT3D_VERTEX, weapon.getAnimVerts(), weapon.getVertDataSize());

	//draw the sword
	glCullFace(GL_FRONT); // md2 faces are defined clockwise, so cull front face
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	tmpMaterial = material1;
	rt3d::setMaterial(shaderProgram, tmpMaterial);
	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), playerPos);
	mvStack.top() = glm::rotate(mvStack.top(), float(90.0f*DEG_TO_RADIAN), glm::vec3(-1.0f, 0.0f, 0.0f));
	mvStack.top() = glm::rotate(mvStack.top(), float(90.0f*DEG_TO_RADIAN - playerRotation / 57.5), glm::vec3(0.0f, 0.0f, 1.0f));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(scale*0.03, scale*0.03, scale*0.03));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawMesh(meshObjects[3], md2VertCount3, GL_TRIANGLES);
	mvStack.pop();
	glCullFace(GL_BACK);

	tmpModel.Animate(enemyAnim, 0.1);
	rt3d::updateMesh(meshObjects[1], RT3D_VERTEX, tmpModel.getAnimVerts(), tmpModel.getVertDataSize());

	//draw the enemy
	glCullFace(GL_FRONT); // md2 faces are defined clockwise, so cull front face
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	tmpMaterial = material1;
	rt3d::setMaterial(shaderProgram, tmpMaterial);
	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), enemyPos);
	mvStack.top() = glm::rotate(mvStack.top(), float(90.0f*DEG_TO_RADIAN), glm::vec3(-1.0f, 0.0f, 0.0f));
	mvStack.top() = glm::rotate(mvStack.top(), float(90.0f*DEG_TO_RADIAN + (180-playerRotation) / 57.5), glm::vec3(0.0f, 0.0f, 1.0f));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(scale*0.08, scale*0.08, scale*0.08));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawMesh(meshObjects[1], md2VertCount, GL_TRIANGLES);
	mvStack.pop();
	glCullFace(GL_BACK);

	character->draw(mvStack.top());
	//weaponTest.draw(mvStack.top(), character.position, character.currentAnimation, character.rotation);

	// remember to use at least one pop operation per push...
	mvStack.pop(); // initial matrix
	glDepthMask(GL_TRUE);

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
		exit (1);
	}
	cout << glGetString(GL_VERSION) << endl;

	init();

	bool running = true; // set running to true
	SDL_Event sdlEvent;  // variable to detect SDL events
	while (running)	{	// the event loop
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
