#pragma once
#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H
#define DEG_TO_RAD 0.017453293
using namespace std;
#include "MazePrefab.h"
#include "FileLoader.h"
#include "Character.h"
#include <ctime>
#include <vector>
//template<typename T>

class Character;

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

class Character;

class MazeGenerator
{
public:
	// FUNCTION DECLARATIONS
	int posX = 0;
	int posY = 0;
	int goalX = 0;
	int goalY = 0;
	int totalCells = ((SIZE - 1) / 2)*((SIZE - 1) / 2);

	Cell Level[SIZE][SIZE];
	MazePrefab* Maze_Tiles[SIZE][SIZE];
	vector<Prefab*> Game_Maze_Prefabs = vector<Prefab*>();
	vector<Character*> Game_Maze_Characters = vector<Character*>();
	//ResourceManager* Resource_Managment = new ResourceManager();
	GLuint TSshaderProgram;

	MazeGenerator() {};
	MazeGenerator(GLuint shaderProgram, ResourceManager s_Resource_Managment) { Initialize(Level, shaderProgram); TSshaderProgram = shaderProgram; }
	~MazeGenerator() {};
	void Initialize(Cell Level[][SIZE], GLuint shaderProgram);
	void GenerateMaze(Cell Level[][SIZE], int &posX, int &posY, int &goalX, int &goalY);
	void SaveMaze();
	void LoadMaze();
	void EnterTheMazetrix(Character* playerCharacter, ResourceManager* resManager);
	void CreateObject(Gameobject* gameObject);
	Character* CreateTarget(Quest* activeQuest, ResourceManager* resManager);
	Character* SpawnCharacter(Character* character);
	void SpawnGameobject(Gameobject* character);
	virtual void SetDepthMap(GLuint s_depthMap);// { depthMapTexture = s_depthMap; };
	virtual void Update() {};
	virtual void draw(glm::mat4 object, GLuint s_shaderProgram, int pass);
	//virtual void ClearMemory<class T>(vector<Character*> &s_ArrList);
	GLuint baseShaderProgram; 
};

#endif