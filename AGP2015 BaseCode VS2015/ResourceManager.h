#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "FileLoader.h"
#include "Gameobject.h"
#include <vector>
#include <SDL.h>

class Holder
{
public:
	Holder() {}
	~Holder() {}

	char* fileName;
};

class MD2Holder : public Holder
{
public:
	MD2Holder() {}
	MD2Holder(char* fileName) { this->fileName = fileName; this->model = this->md2model.ReadMD2Model(fileName); this->md2VertCount = this->md2model.getVertDataSize(); }
	MD2Holder(md2model md2model, GLuint model, GLuint md2VertCount, char* fileName) { this->md2model = md2model;  this->model = model; this->md2VertCount = md2VertCount; this->fileName = fileName; }
	MD2Holder(md2model md2model, GLuint md2VertCount, char* fileName) { this->md2model = md2model; this->md2VertCount = md2VertCount; this->fileName = fileName; this->model = md2model.ReadMD2Model(fileName); }
	MD2Holder(md2model md2model, GLuint md2VertCount, GLuint texture) { this->md2model = md2model; this->md2VertCount = md2VertCount; this->texture = texture; }
	~MD2Holder(){}

	md2model md2model;
	GLuint model;
	GLuint md2VertCount;
	GLuint texture;
};

class OBJHolder : public Holder
{
public:
	OBJHolder() {}
	OBJHolder(GLuint model, GLuint meshCount, char* fileName) { this->model = model; this->meshCount = meshCount; this->fileName = fileName; }
	OBJHolder(GLuint model, GLuint meshCount, GLuint texture) { this->model = model; this->meshCount = meshCount; this->texture = texture; }
	~OBJHolder() {}

	GLuint model;
	GLuint meshCount;
	GLuint texture;
};

class TextureHolder : public Holder
{
public:
	TextureHolder() {}
	TextureHolder(GLuint texture, char* fileName) { this->texture = texture; this->fileName = fileName; }
	~TextureHolder() {}

	GLuint texture;
};


class ResourceManager
{
public:
	ResourceManager() {};
	~ResourceManager() {};

	vector<OBJHolder*> LoadedObjects;
	vector<MD2Holder*> LoadedMD2Models;
	vector<TextureHolder*> LoadedTextures;

	FileLoader* fileLoader = new FileLoader();
	OBJHolder* LoadObject(char *fileName);
	MD2Holder* LoadMD2(char *fileName);
	TextureHolder* LoadTexture(char *fileName);
	int CheckIfLoaded(char *fileName, vector<OBJHolder*> vectorList);
	int CheckIfLoaded(char *fileName, vector<MD2Holder*> vectorList);
	int CheckIfLoaded(char *fileName, vector<TextureHolder*> vectorList);
};

#endif

//vector<string> LoadedFileNames;
//FileLoader* fileLoader = new FileLoader();