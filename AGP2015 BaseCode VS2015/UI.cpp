#include "UI.h"



UI::UI()
{
}


GLuint UI::textToTexture(const char * str, GLuint textID/*, TTF_Font *font SDL_Color colour, GLuint &w,GLuint &h */) {
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

	//modelInfo = Resource_Managment->LoadObject("Models/cube.obj");

	arrowObjects = modelInfo->model;
	arrowIndexCount = modelInfo->meshCount;



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

	TextureInfo = Resource_Managment->LoadTexture("Models/Textures/Logo.bmp");
	textures[5] = TextureInfo->texture;

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

	inventoryInfo = new Inventory;

	titles[0] = createTexture("Quests");
	titles[1] = createTexture("Inventory");

	questText[3] = createTexture("Name: ");
	questText[4] = createTexture("Description: ");
	questText[5] = createTexture("Reward: ");

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	menuText[0] = createTexture("Start");
	menuText[1] = createTexture("Quit");



}

int UI::createTexture(const char * str) {
	GLuint text = 0;
	text = textToTexture(str, text);
	return text;

}

void UI::textBox(GLuint text, GLfloat y, bool drawBox, int nameText) {

	//GLuint nameText = 0;
	//nameText = textToTexture(name, nameText, font);



	glUseProgram(shaderProgram);//Use texture-only shaderProgram for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label
							 //labels[0] = ui->textToTexture("work();", labels[0], textFont);
	stack = glm::mat4(1.0);
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

int UI::updateButton() {
	int x = 80;
	int y = 480;

	while (SDL_PollEvent(&event))
	{

		/* If a button on the mouse is pressed. */
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{

			/* If the left button was pressed. */
			if (event.button.button == SDL_BUTTON(SDL_BUTTON_LEFT))
			{

				cout << event.button.x << "    " << event.button.y << endl;
				for (int i = 0; i < 5; i++)
				{
					if (event.button.x > x && event.button.x < x + 80 && event.button.y > y && event.button.y < y + 60)
					{
						return i += 1;
						cout << "what" << endl;
					}

					x += 80;
					//y += 60;
					//cout << x << "   " << y << endl;
				}

			}

		}
	}
}

void UI::button() {


	glUseProgram(shaderProgram);//Use texture-only shaderProgram for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label

	for (GLfloat i = 0; i < 5; i++) {
		stack = glm::mat4(1.0);
		stack = glm::translate(stack, glm::vec3(-0.7 + (i / 5), -0.7, 0.0f));
		stack = glm::scale(stack, glm::vec3(0.1f, 0.1f, 0.0f));
		rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
		rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
		glBindTexture(GL_TEXTURE_2D, Buttons[(int)i]);
		rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	}

	int attack = updateButton();

	if (attack == 1) combat->Input(&LightAttack());
	if (attack == 2) combat->Input(&HeavyAttack());
	if (attack == 3) combat->Input(&Poison());
	if (attack == 4) combat->Input(&Stun());
	if (attack == 5) combat->Input(&Flee());

	glEnable(GL_DEPTH_TEST);

	/////////out puting the queued attacks//////////

	/*std::vector<C_Attack>::iterator itt = combat->queuedAttacks.begin();
	list<int> displayQueue = list<int>();
	std::list<int>::iterator itt2 = displayQueue.begin();




	for (auto&& combat_Attack : combat->queuedAttacks) {*/
		//cout << itt->attackText << " ";


		



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



	//}

}


void UI::statusBar(GLfloat y, GLuint healthBool, float health) {
	glUseProgram(shaderProgram);//Use texture-only shaderProgram for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label

	stack = glm::mat4(1.0);
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


void UI::updateInven(PlayableCharacter *arnould) {
	if (invenSize != arnould->inventory->items.size()) {
		invenSize = arnould->inventory->items.size();
		itemTextures.clear();
		for (auto&& object_Item : arnould->inventory->items)
		{
			int tmp = createTexture(object_Item->name);
			itemTextures.push_back(tmp);
		}
	}

}



void UI::inventory(PlayableCharacter *arnould) {
	glUseProgram(shaderProgram);//Use texture-only shaderProgram for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label

	arnould->inventory->show();
	updateInven(arnould);


	GLfloat i = 0;
	GLfloat k = 0;

	for (GLfloat j = 0; j < itemTextures.size(); j++) {// auto&& object_Item : itemTextures) {


		stack = glm::mat4(1.0);
		stack = glm::translate(stack, glm::vec3(-0.6 + k, 0.7 - (i), 0.0f));
		stack = glm::scale(stack, glm::vec3(0.2f, 0.1f, 0.0f));
		rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
		rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
		glBindTexture(GL_TEXTURE_2D, itemTextures[j]);
		rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
		i += 0.2;
		if (j == 6 || j == 12 || j == 18) {
			k += 0.5;
			i = 0;
		}
	}


	glEnable(GL_DEPTH_TEST);

}
void UI::initQuests(PlayableCharacter *arnould) {
	if (currentQuest != arnould->currentQuest->ID) {
		currentQuest = arnould->currentQuest->ID;
		questText[0] = createTexture(arnould->currentQuest->name);
		questText[1] = createTexture(arnould->currentQuest->description);
		//char* reward = (char*)arnould->currentQuest->reward;cout << "Quests!" << reward<<  endl;
		questText[2] = createTexture("Place holder");

	}
}

void UI::quest(PlayableCharacter *arnould) {
	initQuests(arnould);

	glUseProgram(shaderProgram);//Use texture-only shaderProgram for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label

	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(0.0f, 0, 0.0f));
	stack = glm::scale(stack, glm::vec3(0.8f, 0.8f, 0.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(-0.7f, 0.7, 0.0f));
	stack = glm::scale(stack, glm::vec3(0.1f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, titles[0]);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(-0.7f, 0.5, 0.0f));
	stack = glm::scale(stack, glm::vec3(0.1f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, questText[3]);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(-0.4f, 0.5, 0.0f));
	stack = glm::scale(stack, glm::vec3(0.2f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, questText[0]);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(-0.6f, 0.3, 0.0f));
	stack = glm::scale(stack, glm::vec3(0.2f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, questText[4]);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(0.1f, 0.3, 0.0f));
	stack = glm::scale(stack, glm::vec3(0.5f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, questText[1]);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);


	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(-0.6f, 0.1, 0.0f));
	stack = glm::scale(stack, glm::vec3(0.2f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, questText[5]);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(-0.1f, 0.1, 0.0f));
	stack = glm::scale(stack, glm::vec3(0.3f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, questText[2]);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

	glEnable(GL_DEPTH_TEST);




}

void UI::QuestMarker(glm::mat4 object, GLuint shader, PlayableCharacter *arnould, vector<Character*> Game_Maze_Characters) {


	glm::vec3 targetPos;

	for (auto&& object_Item : Game_Maze_Characters) {
		if (object_Item->characterName == arnould->currentQuest->ID) {
			targetPos = object_Item->position;
		}
	}

	glUseProgram(shaderProgram);//Use texture-only shaderProgram for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label

	glUseProgram(shader);

	object = glm::translate(object, glm::vec3(targetPos.x, targetPos.y + 2, targetPos.z - 1));
	object = glm::rotate(object, float((90.0f - rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	object = glm::scale(object, glm::vec3(0.3, 0.3, 0.3));
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	rt3d::setUniformMatrix4fv(shader, "modelview", glm::value_ptr(object));
	rt3d::drawIndexedMesh(arrowObjects, arrowIndexCount, GL_TRIANGLES);

	rotation++;

	glEnable(GL_DEPTH_TEST);

}
void UI::mainMenu() {

	glm::vec3 position;

	if (startMenu == true)
	{
		position = glm::vec3(0.0f, 0.0, 0.0f);

	}
	else if (quitMenu == true)
	{
		position = glm::vec3(0.0f, -0.5, 0.0f);
	}

	glUseProgram(shaderProgram);//Use texture-only shaderProgram for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label


	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(0.0f, 0.0, 0.0f));
	stack = glm::scale(stack, glm::vec3(1.0f, 1.0f, 1.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);


	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(position));
	stack = glm::scale(stack, glm::vec3(0.25f, 0.25f, 0.25f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(0.0f, 0.0, 0.0f));
	stack = glm::scale(stack, glm::vec3(0.2f, 0.2f, 0.2f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, menuText[0]);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(glm::vec3(0.0f, -0.5, 0.0f)));
	stack = glm::scale(stack, glm::vec3(0.2f, 0.2f, 0.2f));
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, menuText[1]);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);





	glEnable(GL_DEPTH_TEST);
	update();


}

void UI::update() {
	if (keys[SDL_SCANCODE_W]) {
		//if (startMenu == true) {
		//	quitMenu = true;
		//	startMenu = false;
		//}
		if (quitMenu == true) {
			startMenu = true;
			quitMenu = false;
		}
	}

	if (keys[SDL_SCANCODE_S]) {
		if (startMenu == true) {
			quitMenu = true;
			startMenu = false;
		}
		/*if (quitMenu == true) {
		startMenu = true;
		quitMenu = false;
		}*/
	}

	if (keys[SDL_SCANCODE_RETURN]) {
		if (quitMenu == true) {
			SDL_Quit();
		}
		if (startMenu == true) {
			hub = true;
		}

	}

}

bool UI::triggerHub() {
	return hub;
}





UI::~UI()
{
}
