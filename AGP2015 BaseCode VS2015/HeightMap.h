#ifndef HEIGHTMAP_H	
#define HEIGHTMAP_H	


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>
#include <vector>
#include <GL\glew.h>

// loads in a 2D black and white image and stores its pixel color values in a 2D vector
// where each row represents the 'x' coordinate, each column the 'z' coordinate 
// and the value stored representing the 'y' coordinate
void loadHeightmap(char *file, std::vector <std::vector <GLfloat>> &Hmap);

// genrates Hverts, Hnorms, Hindies, Htexcoords, HvertsCount, HindexCount from the Hmap 
// Hnorms are smooth normals calculated during their generations
void initMesh(std::vector <std::vector <GLfloat>> &Hmap, std::vector <GLfloat> &Hverts,
	std::vector <GLfloat> &Hnorms, std::vector <GLuint> &Hindies,
	std::vector <GLfloat> &Htexcoords, GLuint &HvertsCount, GLuint &HindexCount);


#endif //!HEIGHTMAP_H