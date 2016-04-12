#include "MazeGenerator.h"
#define CUBE_DOWN -2.7
void MazeGenerator::draw(glm::mat4 object,  GLuint s_shaderProgram, int pass)
{
	//glm::mat4 modelview(1.0);

	for (int i = 0; i<SIZE; i++) {
		for (int j = 0; j<SIZE; j++) 
		{
			//cout << " position: " << Maze_Tiles[i][j]->position.x << " y: " << Maze_Tiles[i][j]->position.y << " z: " << Maze_Tiles[i][j]->position.z << endl;
			//Maze_Tiles[i][j]->position.x = i;
			Maze_Tiles[i][j]->draw(object, s_shaderProgram, pass);
		}
	}
}

void MazeGenerator::SpawnCharacter(Character* character)
{
	int xPos = rand() % SIZE;
	int yPos = rand() % SIZE;
	glm::vec3 newPosition = Maze_Tiles[xPos][yPos]->testPosition;
	//while (newPosition.y != CUBE_DOWN)
	do
	{
		xPos = rand() % SIZE;
		yPos = rand() % SIZE;
		newPosition = Maze_Tiles[xPos][yPos]->testPosition;
		//cout << "CUBE DOWN IS: " << CUBE_DOWN << " xPos: " << xPos << " yPos: " << yPos << 
		//	endl << " maze position: " << newPosition.x << " current cube is: " << newPosition.y << " " << newPosition.z << endl << endl;
	} while (newPosition.y == 0.5);//Maze_Tiles[xPos][yPos]->testPosition.y != CUBE_DOWN);

	//system("pause");
	character->position.x = newPosition.x;
	character->position.z = newPosition.z;
}

// INITIALIZE MAZE
void MazeGenerator::Initialize(Cell Level[][SIZE], GLuint shaderProgram) 
{
	ResourceManager* Resource_Managment = new ResourceManager();
	Prefab prefab = Prefab(shaderProgram, Resource_Managment->LoadObject("cube.obj"), Resource_Managment->LoadTexture("lava_cube.bmp"), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 0, glm::vec3(-62, 6.8, -35), glm::vec3(-65.5, -1.0, -42));//Prefab(shaderProgram, "Models/House_001.obj", "Models/Textures/Terrain_Sand.bmp", glm::vec3(0.03, 0.02, 0.03), character->position); Prefab(shaderProgram, "cube.obj", "lava_cube.bmp", glm::vec3(1, 1, 1), glm::vec3(1, 1, 1));

	for (int i = 1; i<=SIZE; i++) {
		for (int j = 1; j<=SIZE; j++) {
			Level[i-1][j-1].display = '*';
			Level[i-1][j-1].visited = false;
			Level[i-1][j-1].top_wall = true;
			Level[i-1][j-1].bot_wall = true;
			Level[i-1][j-1].left_wall = true;
			Level[i-1][j-1].right_wall = true;
			Maze_Tiles[i-1][j-1] = new MazePrefab(shaderProgram, prefab.getObject(), prefab.meshIndexCount, prefab.texture, glm::vec3(3, 1.6, 3), glm::vec3(i * 6, 0.5, j * 6));
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
	GenerateMaze(Level, posX, posY, goalX, goalY);
}

void MazeGenerator::GenerateMaze(Cell Level[][SIZE], int &posX, int &posY, int &goalX, int &goalY) {
	srand((unsigned)time(NULL));                                                                            // Pick random start cell
	int random = 0;
	int randomX = ((2 * rand()) + 1) % (SIZE - 1);                                          // Generate a random odd number between 1 and SIZE
	int randomY = ((2 * rand()) + 1) % (SIZE - 1);                                          // Generate a random odd number between 1 and SIZE
	posX = randomX * 3;                                                                 // Save player's initial X position
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
					Maze_Tiles[randomY - 1][randomX]->testPosition.y = CUBE_DOWN;
					Level[randomY - 1][randomX].visited = true;       // Mark cell as visited
					Level[randomY][randomX].top_wall = false;       // Knock down wall

					back_trackX.push(randomX);                      // Push X for back track
					back_trackY.push(randomY);                      // Push Y for back track

					randomY -= 2;                                   // Move to next cell
					Level[randomY][randomX].visited = true;         // Mark cell moved to as visited
					Level[randomY][randomX].display = '_';          // Update path
					Maze_Tiles[randomY][randomX]->testPosition.y = CUBE_DOWN;
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
					Maze_Tiles[randomY+1][randomX]->testPosition.y = CUBE_DOWN;
					Level[randomY + 1][randomX].visited = true;       // Mark cell as visited
					Level[randomY][randomX].bot_wall = false;       // Knock down wall

					back_trackX.push(randomX);                      // Push X for back track
					back_trackY.push(randomY);                      // Push Y for back track

					randomY += 2;                                   // Move to next cell
					Level[randomY][randomX].visited = true;         // Mark cell moved to as visited
					Level[randomY][randomX].display = '_';          // Update path
					Maze_Tiles[randomY][randomX]->testPosition.y = CUBE_DOWN;
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
					Maze_Tiles[randomY][randomX-1]->testPosition.y = CUBE_DOWN;
					Level[randomY][randomX - 1].visited = true;       // Mark cell as visited
					Level[randomY][randomX].left_wall = false;      // Knock down wall

					back_trackX.push(randomX);                      // Push X for back track
					back_trackY.push(randomY);                      // Push Y for back track

					randomX -= 2;                                   // Move to next cell
					Level[randomY][randomX].visited = true;         // Mark cell moved to as visited
					Level[randomY][randomX].display = '_';          // Update path
					Maze_Tiles[randomY][randomX]->testPosition.y = CUBE_DOWN;
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
					Maze_Tiles[randomY][randomX + 1]->testPosition.y = CUBE_DOWN;
					Level[randomY][randomX + 1].visited = true;       // Mark cell as visited
					Level[randomY][randomX].right_wall = false;     // Knock down wall

					back_trackX.push(randomX);                      // Push X for back track
					back_trackY.push(randomY);                      // Push Y for back track

					randomX += 2;                                   // Move to next cell
					Level[randomY][randomX].visited = true;         // Mark cell moved to as visited
					Level[randomY][randomX].display = '_';          // Update path
					Maze_Tiles[randomY][randomX]->testPosition.y = CUBE_DOWN;
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

	}

	goalX = randomX;
	goalY = randomY;
	Level[goalY][goalX].display = '.';
}

void MazeGenerator::SaveMaze()
{
	ofstream output;
	char file[20] = "MazeSeed";
	char input;
	int passes = 0;
	cout << endl << "Saving Maze....";

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

void MazeGenerator::LoadMaze()
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
	}
	else { cout << "Unable to open file"; }
}

void MazeGenerator::SetDepthMap(GLuint s_depthMap)
{
	for (int i = 0; i<SIZE; i++) {
		for (int j = 0; j<SIZE; j++) {
			Maze_Tiles[i][j]->SetDepthMap(s_depthMap);
		}
	}
};