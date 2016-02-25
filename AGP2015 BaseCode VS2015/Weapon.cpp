#include "Weapon.h"

Weapon::Weapon(string s_objectName, char *modelName, char *textureName, int s_cost, float s_damage, float s_speed, string s_type, int s_rarity, GLuint s_shaderprogram)
{
	collisionName = s_objectName;
	shaderProgram = s_shaderprogram;
	material =
	{
		{ 0.4f, 0.2f, 0.2f, 1.0f }, // ambient
		{ 0.8f, 0.5f, 0.5f, 1.0f }, // diffuse
		{ 1.0f, 0.8f, 0.8f, 1.0f }, // specular
		2.0f  // shininess
	};

	FileLoader* fileLoader = new FileLoader;
	//texture = fileLoader->loadBitmap("hobgoblin2.bmp");//textureName);
	//texture = fileLoader->loadBitmap("lava_cube.bmp");


	/*samples = new HSAMPLE[4];
	samples[0] = fileLoader->loadSample("Sound/Coin.wav");
	samples[1] = fileLoader->loadSample("Sound/Hurt.wav");
	samples[2] = fileLoader->loadSample("Sound/Dead.wav");
	samples[3] = fileLoader->loadSample("Sound/Win.wav");*/
	delete fileLoader;

	//meshObject = tmpModel.ReadMD2Model("arnould.MD2");
	//md2VertCount = tmpModel.getVertDataSize();
	meshObject = tmpModel.ReadMD2Model("Partical_sword.MD2");
	md2VertCount = tmpModel.getVertDataCount();
}

void Weapon::InitalStats(GLuint s_shaderprogram)
{
	shaderProgram = s_shaderprogram;
	FileLoader* fileLoader = new FileLoader;
	//texture = fileLoader->loadBitmap("hobgoblin2.bmp");//textureName);
	texture = fileLoader->loadBitmap("lava_cube.bmp");
	scale = glm::vec3(1, 1, 1);
	position = glm::vec3(3, 3, 3);

	/* Initialize default output device */
	if (!BASS_Init(-1, 44100, 0, 0, NULL))
		std::cout << "Can't initialize device";

	/*samples = new HSAMPLE[4];
	samples[0] = fileLoader->loadSample("Sound/Coin.wav");
	samples[1] = fileLoader->loadSample("Sound/Hurt.wav");
	samples[2] = fileLoader->loadSample("Sound/Dead.wav");
	samples[3] = fileLoader->loadSample("Sound/Win.wav");*/
	delete fileLoader;

	//meshObject = tmpModel.ReadMD2Model("arnould.MD2");
	//md2VertCount = tmpModel.getVertDataSize();
	meshObject = tmpModel.ReadMD2Model("Partical_sword.MD2");
	md2VertCount = tmpModel.getVertDataCount();
}


void Weapon::draw(glm::mat4 object, glm::vec3 playerPosition, int currentAnimation, int playerRotation)
{

	glUseProgram(shaderProgram);
	glCullFace(GL_FRONT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Animation
	//tmpModel.Animate(currentAnimation, 0.1);
	//rt3d::updateMesh(meshObject, RT3D_VERTEX, tmpModel.getAnimVerts(), tmpModel.getVertDataSize());

	//// drawing the player model

	////glBindTexture(GL_TEXTURE_2D, texture);
	//object = glm::translate(object, playerPosition);
	//object = glm::rotate(object, float(90.0f*DEG_TO_RAD), glm::vec3(-1.0f, 0.0f, 0.0f));
	//object = glm::rotate(object, float(90.0f*DEG_TO_RAD - playerRotation / 57.5), glm::vec3(0.0f, 0.0f, 1.0f));
	//object = glm::scale(object, glm::vec3(scale.x*0.03, scale.y*0.03, scale.z*0.03));
	////object = glm::rotate(object, float(90.0f*DEG_TO_RAD), glm::vec3(-1.0f, 0.0f, 0.0f));

	//rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	//rt3d::drawMesh(meshObject, md2VertCount, GL_TRIANGLES);
	//glCullFace(GL_BACK);

	tmpModel.Animate(currentAnimation, 0.1);
	rt3d::updateMesh(meshObject, RT3D_VERTEX, tmpModel.getAnimVerts(), tmpModel.getVertDataSize());
	glBindTexture(GL_TEXTURE_2D, texture);
	//rt3d::setMaterial(shaderProgram, material);

	object = glm::translate(object, glm::vec3(playerPosition.x, playerPosition.y, playerPosition.z-1));
	object = glm::rotate(object, float(90.0f*DEG_TO_RAD), glm::vec3(-1.0f, 0.0f, 0.0f));
	object = glm::rotate(object, float(90.0f*DEG_TO_RAD - playerRotation / 57.5), glm::vec3(0.0f, 0.0f, 1.0f));
	object = glm::scale(object, glm::vec3(scale.x*0.05, scale.y*0.05, scale.z*0.05));

	//object = glm::translate(object, playerPosition);
	//object = glm::rotate(object, float(90.0f*DEG_TO_RAD), glm::vec3(-1.0f, 0.0f, 0.0f));
	//object = glm::rotate(object, float(90.0f*DEG_TO_RAD - playerRotation / 57.5), glm::vec3(0.0f, 0.0f, 1.0f));
	//object = glm::scale(object, glm::vec3(scale.x*0.05, scale.y*0.05, scale.z*0.05));

	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(object));
	rt3d::drawMesh(meshObject, md2VertCount, GL_TRIANGLES);
	glCullFace(GL_BACK);
}