#include "Skybox.h"


GLuint loadCubeMapTex(const char *fname[6], GLuint *texID)
{
	glGenTextures(1, texID); // generate texture ID
	GLenum sides[6] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y };
	SDL_Surface *tmpSurface;

	glBindTexture(GL_TEXTURE_CUBE_MAP, *texID); // bind texture and set parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	GLuint externalFormat;
	for (int i = 0; i<6; i++)
	{
		// load file - using core SDL library
		tmpSurface = SDL_LoadBMP(fname[i]);
		if (!tmpSurface)
		{
			std::cout << "Error loading bitmap" << std::endl;
			return *texID;
		}

		// skybox textures should not have alpha (assuming this is true!)
		SDL_PixelFormat *format = tmpSurface->format;
		externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;

		glTexImage2D(sides[i], 0, GL_RGB, tmpSurface->w, tmpSurface->h, 0,
			externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
		// texture loaded, free the temporary buffer
		SDL_FreeSurface(tmpSurface);
	}
	return *texID;	// return value of texure ID, redundant really
}

Skybox::Skybox(const char *skyboxmap[6])
{
	shaderProgram = rt3d::initShaders("cubeMap.vert", "cubeMap.frag");

	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj("cube.obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
	loadCubeMapTex(skyboxmap, &textures[0]);
}


/* Sets inital stats for the skybox. Shaderprogram is passed in from light class.
Textures are loaded into array of textures - (skybox taken from lab tutorial).*/
void Skybox::InitalStats(const char *skyboxmap[6])
{
	shaderProgram = rt3d::initShaders("cubeMap.vert", "cubeMap.frag");

	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	rt3d::loadObj("cube.obj", verts, norms, tex_coords, indices);
	GLuint size = indices.size();
	meshIndexCount = size;
	meshObject = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
	loadCubeMapTex(skyboxmap, &textures[0]);
}

/* Handles draw event for the skybox.
-Sets depthMask to false, creates modelview and pushes it to stack top.
-Each side of the skybox is drawn from the stack.
After all sides are drawn, depth mask is reset to true. */
void Skybox::draw(glm::mat4 object)
{

	glUseProgram(shaderProgram);
	//glm::mat4 modelview(1.0);
	mvStack.push(object);
	mvStack.push(mvStack.top());
	glDepthMask(GL_FALSE); // make sure writing to update depth test is off
	glm::mat3 mvRotOnlyMat3 = glm::mat3(mvStack.top());
	mvStack.push(glm::mat4(mvRotOnlyMat3));

	glCullFace(GL_FRONT); // drawing inside of cube!
	glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(1.5f, 1.5f, 1.5f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	mvStack.pop();
	glCullFace(GL_BACK); // drawing inside of cube!


						 // back to remainder of rendering
	glDepthMask(GL_TRUE); // make sure depth test is on

	//mvStack.push(object);
	//mvStack.push(mvStack.top());
	//glUseProgram(shaderProgram);
	//glDepthMask(GL_FALSE); // make sure writing to update depth test is off
	//glm::mat3 mvRotOnlyMat3 = glm::mat3(mvStack.top());
	//mvStack.push(glm::mat4(mvRotOnlyMat3));
	//glCullFace(GL_FRONT); // drawing inside of cube!
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
	//mvStack.top() = glm::scale(mvStack.top(), glm::vec3(1.5f, 1.5f, 1.5f));
	//rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	//rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	//mvStack.pop();
	//glCullFace(GL_BACK); // drawing inside of cube!
	//glDepthMask(GL_TRUE); // make sure depth test is on*/

	//glDepthMask(GL_FALSE);
	//glm::mat4 modelview(1.0);
	//mvStack.push(object);

	//mvStack.push(mvStack.top());
	//glBindTexture(GL_TEXTURE_2D, textures[0]);
	//object = glm::scale(object, glm::vec3(2.0f, 2.0f, 2.0f));
	//object = glm::translate(object, glm::vec3(0.0f, 0.0f, -2.0f));
	//rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	//rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	//mvStack.pop();

	//mvStack.push(mvStack.top());
	//glBindTexture(GL_TEXTURE_2D, textures[1]);
	//mvStack.top() = glm::scale(mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	//mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 0.0f, 2.0f));
	//rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	//rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	//mvStack.pop();

	//mvStack.push(mvStack.top());
	//glBindTexture(GL_TEXTURE_2D, textures[2]);
	//mvStack.top() = glm::scale(mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	//mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-2.0f, 0.0f, 0.0f));
	//rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	//rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	//mvStack.pop();

	//mvStack.push(mvStack.top());
	//glBindTexture(GL_TEXTURE_2D, textures[3]);
	//mvStack.top() = glm::scale(mvStack.top(), glm::vec3(2.0f, 2.0f, 2.0f));
	//mvStack.top() = glm::translate(mvStack.top(), glm::vec3(2.0f, 0.0f, 0.0f));
	//rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	//rt3d::drawIndexedMesh(meshObject, meshIndexCount, GL_TRIANGLES);
	//mvStack.pop();

	//glDepthMask(GL_TRUE);
}