#pragma once
#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H
#define DEG_TO_RAD 0.017453293
using namespace std;
#include "Prefab.h"
#include "FileLoader.h"
#include <ctime>
#include <vector>

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
	Prefab Maze_Tiles[SIZE][SIZE];
	//vector<Prefab> Maze_Tiles;

	MazeGenerator() {};
	~MazeGenerator() {};
	void Initialize(Cell Level[][SIZE]);
	void GenerateMaze(Cell Level[][SIZE], int &posX, int &posY, int &goalX, int &goalY);
	void SaveMaze();
	void LoadMaze();
	virtual void draw(glm::mat4 object);
};

#endif