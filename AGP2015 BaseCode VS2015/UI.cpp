#include "UI.h"



UI::UI()
{
}


GLuint UI::textToTexture(const char * str, GLuint textID, TTF_Font *font /*SDL_Color colour, GLuint &w,GLuint &h */) {
	TTF_Font *textFont = font;
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

}

int UI::createTexture(const char * str, TTF_Font *font) {
	GLuint text = 0;
	text = textToTexture(str, text, font);
	return text;

}

void UI::textBox(GLuint text, GLuint shader, GLfloat y, GLuint texture, bool drawBox, int nameText) {

	//GLuint nameText = 0;
	//nameText = textToTexture(name, nameText, font);



	glUseProgram(shader);//Use texture-only shader for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label
							 //labels[0] = ui->textToTexture("work();", labels[0], textFont);
	glm::mat4 stack = glm::mat4(1.0);
	if (drawBox)
	{

		stack = glm::translate(stack, glm::vec3(-0.0f, -0.6, 0.0f));
		stack = glm::scale(stack, glm::vec3(1.00f, 0.2f, 0.0f));
		rt3d::setUniformMatrix4fv(shader, "projection", glm::value_ptr(glm::mat4(1.0)));
		rt3d::setUniformMatrix4fv(shader, "modelview", glm::value_ptr(stack));
		glBindTexture(GL_TEXTURE_2D, texture);
		rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	}

	glBindTexture(GL_TEXTURE_2D, text);

	stack = glm::mat4(1.0);

	stack = glm::translate(stack, glm::vec3(-0.0f, y, 0.0f));
	stack = glm::scale(stack, glm::vec3(1.00f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(shader, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shader, "modelview", glm::value_ptr(stack));
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);



	glBindTexture(GL_TEXTURE_2D, nameText);

	stack = glm::mat4(1.0);

	stack = glm::translate(stack, glm::vec3(-0.8f, -0.4, 0.0f));
	stack = glm::scale(stack, glm::vec3(0.1f, 0.1f, 0.0f));
	rt3d::setUniformMatrix4fv(shader, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shader, "modelview", glm::value_ptr(stack));
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	glEnable(GL_DEPTH_TEST);//Re-enable depth test after HUD label 


}

void UI::button(GLuint shader, GLuint texture, GLuint button, GLuint time) {
	glUseProgram(shader);//Use texture-only shader for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label

	for (GLfloat i = 0; i < time; i++) {
		glm::mat4 stack = glm::mat4(1.0);
		stack = glm::translate(stack, glm::vec3(0.0f, 0, 0.0f));
		stack = glm::scale(stack, glm::vec3(0.4f / i, 0.4f / i, 0.0f));
		rt3d::setUniformMatrix4fv(shader, "projection", glm::value_ptr(glm::mat4(1.0)));
		rt3d::setUniformMatrix4fv(shader, "modelview", glm::value_ptr(stack));
		glBindTexture(GL_TEXTURE_2D, texture);
		rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);


		stack = glm::mat4(1.0);
		stack = glm::translate(stack, glm::vec3(0.0f, 0, 0.0f));
		stack = glm::scale(stack, glm::vec3(0.2f / i, 0.2f / i, 0.0f));
		rt3d::setUniformMatrix4fv(shader, "projection", glm::value_ptr(glm::mat4(1.0)));
		rt3d::setUniformMatrix4fv(shader, "modelview", glm::value_ptr(stack));
		glBindTexture(GL_TEXTURE_2D, button);
		rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

	}
	glEnable(GL_DEPTH_TEST);
}


void UI::statusBar(GLuint shader, GLfloat y, GLuint texture, GLuint texture2, float health) {
	glUseProgram(shader);//Use texture-only shader for text rendering
	glDisable(GL_DEPTH_TEST);//Disable depth test for HUD label

	glm::mat4 stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(-0.4f, y, 0.0f));
	stack = glm::scale(stack, glm::vec3(0.51f, 0.04f, 0.0f));
	rt3d::setUniformMatrix4fv(shader, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shader, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, texture2);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);

	stack = glm::mat4(1.0);
	stack = glm::translate(stack, glm::vec3(-0.4f, y, 0.0f));
	stack = glm::scale(stack, glm::vec3(health, 0.03f, 0.0f));
	rt3d::setUniformMatrix4fv(shader, "projection", glm::value_ptr(glm::mat4(1.0)));
	rt3d::setUniformMatrix4fv(shader, "modelview", glm::value_ptr(stack));
	glBindTexture(GL_TEXTURE_2D, texture);
	rt3d::drawIndexedMesh(meshObjects, meshIndexCount, GL_TRIANGLES);
	glEnable(GL_DEPTH_TEST);

}


UI::~UI()
{
}
