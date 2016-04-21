#include "UI.h"



UI::UI()
{
}


GLuint UI::textToTexture(const char * str, GLuint textID, TTF_Font *font /*SDL_Color colour, GLuint &w,GLuint &h */) {
	//TTF_Font *textFont = font;
	SDL_Color colour = { 255, 0, 255 };
	SDL_Color bg = { 0, 0, 0 };

	SDL_Surface *stringImage;
	stringImage = TTF_RenderText_Blended(textFont, str, colour);

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
	}
	else {             // no alpha
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


void UI::loadRect() {
	modelInfo = Resource_Managment->LoadObject("cube.obj");

	meshObjects = modelInfo->model;
	meshIndexCount = modelInfo->meshCount;


	if (TTF_Init() == -1)
		cout << "TTF failed to initialise." << endl;

	textFont = TTF_OpenFont("ESKARGOT.ttf", 48);
	if (textFont == NULL)
		cout << "Failed to open font." << endl;

	TextureInfo = Resource_Managment->LoadTexture("Models/Textures/textbox.bmp");
	textures[0] = TextureInfo->texture;

	TextureInfo = Resource_Managment->LoadTexture("Models/Textures/health texture.bmp");
	textures[1] = TextureInfo->texture;

	TextureInfo = Resource_Managment->LoadTexture("Models/Textures/StatusBar.bmp");
	textures[2] = TextureInfo->texture;

	TextureInfo = Resource_Managment->LoadTexture("Models/Textures/mana texture.bmp");
	textures[3] = TextureInfo->texture;

	TextureInfo = Resource_Managment->LoadTexture("Models/Textures/Button.bmp");
	textures[4] = TextureInfo->texture;

	TextureInfo = Resource_Managment->LoadTexture("Models/Textures/Button_Light_Attack.bmp");
	Buttons[0] = TextureInfo->texture;

	TextureInfo = Resource_Managment->LoadTexture("Models/Textures/Button_Heavy_Attack.bmp");
	Buttons[1] = TextureInfo->texture;

	TextureInfo = Resource_Managment->LoadTexture("Models/Textures/Button_Poison.bmp");
	Buttons[2] = TextureInfo->texture;

	TextureInfo = Resource_Managment->LoadTexture("Models/Textures/Button_Stun.bmp");
	Buttons[3] = TextureInfo->texture;

	TextureInfo = Resource_Managment->LoadTexture("Models/Textures/Button_Flee.bmp");
	Buttons[4] = TextureInfo->texture;

	shaderProgram = rt3d::initShaders("textured.vert", "textured.frag");

	combat = new CombatInstance;




}

int UI::createTexture(const char * str, TTF_Font *font) {
	GLuint text = 0;
	text = textToTexture(str, text, font);
	return text;

}

void UI::textBox(GLuint text, GLfloat y, bool drawBox, int nameText) {

	//GLuint nameText = 0;
	//nameText = textToTexture(name, nameText, font);



	glUseProgram(shaderProgram);//Use texture-only shaderProgram for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label
							 //labels[0] = ui->textToTexture("work();", labels[0], textFont);
	glm::mat4 stack = glm::mat4(1.0);
	if (drawBox)
	{

		stack = glm::translate(stack, glm::vec3(-0.0f, -0.6, 0.0f));
		stack = glm::scale(stack, glm::vec3(1.00f, 0.2f, 0.0f));
		rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
		rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	}

	glBindTexture(GL_TEXTURE_2D, text);

	stack = glm::mat4(1.0);

	stack = glm::translate(stack, glm::vec3(-0.0f, y, 0.0f));
	stack = glm::scale(stack, glm::vec3(1.00f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);



	glBindTexture(GL_TEXTURE_2D, nameText);

	stack = glm::mat4(1.0);

	stack = glm::translate(stack, glm::vec3(-0.8f, -0.4, 0.0f));
	stack = glm::scale(stack, glm::vec3(0.1f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	glEnable(GL_DEPTH_TEST);//Re-enable depth test after HUD label 


}

void UI::button(GLuint button, GLuint time) {


	glUseProgram(shaderProgram);//Use texture-only shaderProgram for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label

	for (GLfloat i = 0; i < 5; i++) {
		glm::mat4 stack = glm::mat4(1.0);
		stack = glm::translate(stack, glm::vec3(-0.7 + (i / 5), -0.7, 0.0f));
		stack = glm::scale(stack, glm::vec3(0.1f, 0.1f, 0.0f));
		rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
		rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
		glBindTexture(GL_TEXTURE_2D, Buttons[(int)i]);
		rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	}

	if (keys[SDL_SCANCODE_1]) combat->Input(LightAttack());
	if (keys[SDL_SCANCODE_2]) combat->Input(HeavyAttack());
	if (keys[SDL_SCANCODE_3]) combat->Input(Poison());
	if (keys[SDL_SCANCODE_4]) combat->Input(Stun());
	if (keys[SDL_SCANCODE_5]) combat->Input(Flee());



	/////////out puting the queued attacks//////////

	std::list<C_Attack>::iterator itt = combat->queuedAttacks.begin();
	list<int> displayQueue = list<int>();
	std::list<int>::iterator itt2 = displayQueue.begin();




	for (itt = combat->queuedAttacks.begin(); itt != combat->queuedAttacks.end(); itt++) {
	//	cout << itt->attackText << " ";

		/////////////////Code for queued attack list display thing////////////////







		//	int texture = combat->queuedAttacks.size();



		//	if (itt->attackText == "Light Attack" && itt->attackCompleted == false) {
		//		texture = 0;
		//		displayQueue.push_back(0);
		//	}
		//	else
		//		if (itt->attackText == "Heavy Attack" && itt->attackCompleted == false) {
		//			texture = 1;
		//			displayQueue.push_back(1);
		//		}
		//	if (itt->attackText == "Poison" && itt->attackCompleted == false) {
		//		texture = 2;
		//		displayQueue.push_back(2);
		//	}
		//	if (itt->attackText == "Stun" && itt->attackCompleted == false) {
		//		texture = 3;
		//		displayQueue.push_back(3);
		//	}
		//	if (itt->attackText == "Flee" && itt->attackCompleted == false) {
		//		texture = 4;
		//		displayQueue.push_back(4);
		//	}

		//	for (GLfloat i = 0; i < combat->queuedAttacks.size(); i++) {
		//		for (itt2 = displayQueue.begin(); itt2 != displayQueue.end(); itt2++) {


		//			glm::mat4 stack = glm::mat4(1.0);
		//			stack = glm::translate(stack, glm::vec3(-0.7 + (i / 5), -0.4, 0.0f));
		//			stack = glm::scale(stack, glm::vec3(0.1f, 0.1f, 0.0f));
		//			rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
		//			rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
		//			glBindTexture(GL_TEXTURE_2D, Buttons[*itt2]);
		//			rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
		//			cout << *itt2 << endl;

		//		}
		//	}


			//itt->attackCompleted

		//}

		glEnable(GL_DEPTH_TEST);



		//
		//start = std::clock();
		//
		//double duration = 0;




		//cout << "drawing button" << endl;

		//int timeLimit = 1;
		//int scale = 1;
		//GLfloat timeDifference = 0;
		////cout << duration << "  " << timeLimit << endl;

		//
		//

		//do {
		//	glUseProgram(shaderProgram);//Use texture-only shaderProgram for text rendering
		//	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label
		//	
		//		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;


		//		glm::mat4 stack = glm::mat4(1.0);
		//		stack = glm::translate(stack, glm::vec3(0.0f, 0, 0.0f));
		//		stack = glm::scale(stack, glm::vec3(0.4f /scale, 0.4f / scale, 0.0f));
		//		rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
		//		rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
		//		glBindTexture(GL_TEXTURE_2D, textures[4]);
		//		rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);


		//		stack = glm::mat4(1.0);
		//		stack = glm::translate(stack, glm::vec3(0.0f, 0, 0.0f));
		//		stack = glm::scale(stack, glm::vec3(0.2f / scale, 0.2f / scale, 0.0f));
		//		rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
		//		rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
		//		glBindTexture(GL_TEXTURE_2D, button);
		//		rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

		//		

		//		if (duration >= timeLimit) {
		//			scale++;
		//			timeLimit += 1;
		//			cout << "if statment" << endl; 

		//		}
		//		
		//		
		//		

		//		cout << timeLimit << "  " << duration << "  " << start << endl;
		//		cout << "While statmnent" << endl;
		//		


		//}while (duration < time);

		//



		//glEnable(GL_DEPTH_TEST);
	}

}
	void UI::statusBar(GLfloat y, GLuint healthBool, float health) {
		glUseProgram(shaderProgram);//Use texture-only shaderProgram for text rendering
		glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label

		glm::mat4 stack = glm::mat4(1.0);
		stack = glm::translate(stack, glm::vec3(-0.4f, y, 0.0f));
		stack = glm::scale(stack, glm::vec3(0.51f, 0.04f, 0.0f));
		rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
		rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

		stack = glm::mat4(1.0);
		stack = glm::translate(stack, glm::vec3(-0.4f, y, 0.0f));
		stack = glm::scale(stack, glm::vec3(health, 0.03f, 0.0f));
		rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
		rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
		if (healthBool == 0)
			glBindTexture(GL_TEXTURE_2D, textures[1]);
		if (healthBool == 1)
			glBindTexture(GL_TEXTURE_2D, textures[3]);
		rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
		glEnable(GL_DEPTH_TEST);

	}


	UI::~UI()
	{
	}
