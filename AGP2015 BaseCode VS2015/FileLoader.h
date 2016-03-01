#ifndef FILELOADER_H
#define FILELOADER_H
#include "rt3d.h"
#include "rt3dObjLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include "md2model.h"
#include "bass.h"
#define DEG_TO_RAD 0.017453293
//#include "FileLoader.h"
//#include <SDL.h>

class FileLoader
{
public:
	FileLoader();
	~FileLoader(){};
	GLuint loadBitmap(char *fname, bool repeatTexture = false);
	HSAMPLE loadSample(char * filename);
	//HSAMPLE *samples = NULL;
};

#endif
