#include "ResourceManager.h"

OBJHolder* ResourceManager::LoadObject(char *fileName)
{
	int valueFound = CheckIfLoaded(fileName, LoadedObjects);
	if (valueFound == -1)
	{
		vector<GLfloat> verts;
		vector<GLfloat> norms;
		vector<GLfloat> tex_coords;
		vector<GLuint> indices;
		rt3d::loadObj(fileName, verts, norms, tex_coords, indices);
		GLuint meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), indices.size(), indices.data());
		LoadedObjects.push_back(new OBJHolder(meshObject, indices.size(), fileName));
		return LoadedObjects[LoadedObjects.size()-1];
	}
	else {
		return LoadedObjects[valueFound];
	}
}

MD2Holder* ResourceManager::LoadMD2(char *fileName)
{
	int valueFound = CheckIfLoaded(fileName, LoadedMD2Models);
	if (valueFound == -1)
	{
		md2model tmpModel; 
		GLuint meshObject = tmpModel.ReadMD2Model(fileName);
		GLuint md2VertCount = tmpModel.getVertDataSize();
		LoadedMD2Models.push_back(new MD2Holder(fileName));
		//LoadedMD2Models.push_back(new MD2Holder(tmpModel, meshObject, md2VertCount, fileName));
		//LoadedMD2Models.push_back(new MD2Holder(tmpModel, md2VertCount, fileName));
		return LoadedMD2Models[LoadedMD2Models.size()-1];
	} else {
		return LoadedMD2Models[valueFound];
	}
}

TextureHolder* ResourceManager::LoadTexture(char *fileName)
{
	int valueFound = CheckIfLoaded(fileName, LoadedMD2Models);
	if (valueFound == -1)
	{
		LoadedTextures.push_back(new TextureHolder(fileLoader->loadBitmap(fileName), fileName));
		return LoadedTextures[LoadedTextures.size()-1];
	}
	else {
		return LoadedTextures[valueFound];
	}
}


int ResourceManager::CheckIfLoaded(char *fileName, vector<OBJHolder*> vectorList)
{
	vector<OBJHolder*>::iterator iter;
	int indexCount = 0;
	for (iter = vectorList.begin(); iter != vectorList.end(); iter++)
	{
		if ((*iter)->fileName == fileName)
		{
			return indexCount;
			break;
		}
		indexCount++;
	}
	return -1;
}

int ResourceManager::CheckIfLoaded(char *fileName, vector<MD2Holder*> vectorList)
{
	vector<MD2Holder*>::iterator iter;
	int indexCount = 0;
	for (iter = vectorList.begin(); iter != vectorList.end(); iter++)
	{
		if ((*iter)->fileName == fileName)
		{
			return indexCount;
			break;
		}
		indexCount++;
	}
	return -1;
}

int ResourceManager::CheckIfLoaded(char *fileName, vector<TextureHolder*> vectorList)
{
	vector<TextureHolder*>::iterator iter;
	int indexCount = 0;
	for (iter = vectorList.begin(); iter != vectorList.end(); iter++)
	{
		if ((*iter)->fileName == fileName)
		{
			return indexCount;
			break;
		}
		indexCount++;
	}
	return -1;
}