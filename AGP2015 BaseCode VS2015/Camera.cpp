#include "Camera.h"
#define DEG_TO_RAD 0.017453293

/* Updates camera position. Same function used in character class. */
glm::vec3 Camera::MoveForward(glm::vec3 cam, GLfloat angle, GLfloat d) {
	return glm::vec3(cam.x + d*std::sin(angle*DEG_TO_RAD),
		cam.y, cam.z - d*std::cos(angle*DEG_TO_RAD));
}

/*The camera information is not loaded from a file, instead hardcoded.*/
void Camera::InitalStats()
{
	eye = { 0.0f, 2.5f, 10.0f };
	at = { 0.0f, 1.0f, -6.0 };
	up = { 0.0f, 1.0f, 0.0f };

/*	FileLoader* fileLoader = new FileLoader;
	 //Initialize default output device 
	if (!BASS_Init(-1, 44100, 0, 0, NULL))
		std::cout << "Can't initialize device";

	songs = new HSAMPLE[4];
	songs[0] = fileLoader->loadSample("Sound/TOM.wav");
	delete fileLoader;*/
}

/*void Camera::Sound(int soundFile)
{
	HCHANNEL ch = BASS_SampleGetChannel(songs[soundFile], FALSE);
	BASS_ChannelSetAttribute(ch, BASS_ATTRIB_FREQ, 0);
	BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, 0.5);
	BASS_ChannelSetAttribute(ch, BASS_ATTRIB_PAN, -1);
	if (!BASS_ChannelPlay(ch, FALSE))
		std::cout << "Can't play sample" << std::endl;

}*/

/* Handles draw method for camera. Camera is always looking directly behind the player. */
void Camera::draw(glm::mat4 &object, glm::vec3 modelEye)
{
	switch (camera_Type)
	{
	case 1:
		at = MoveForward(modelEye, 0, 1.0f);
		break;
	case 2:
		at = MoveForward(modelEye, 0, 1.0f);
		break;
	case 3:
		at = MoveForward(eye, rotation, 1.0f);
		break;
	default:
		at = MoveForward(modelEye, 0, 1.0f);
		break;
	}

	object = glm::lookAt(eye, at, up);
	object = glm::rotate(object, float((rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	object = glm::translate(object, -modelEye);
}


/* Update the camera position to make it stay behind the player model at all times. */
void Camera::update(glm::vec3 modelEye, float playerRotation)
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_F12])
	{
		camera_Type++;
		if (camera_Type > 3)
			camera_Type = 1;

		cout << camera_Type << endl;
	}

	switch (camera_Type)
	{
	case 1:
		eye.x = modelEye.x + 6.0f;
		eye.y = modelEye.y + 20.0f;
		eye.z = modelEye.z + 10.0f;
		break;
	case 2:

		break;
	case 3:
		if (keys[SDL_SCANCODE_W])
		{
			modelEye = MoveForward(modelEye, rotation, 0.2f);
		} else if (keys[SDL_SCANCODE_S]){
			modelEye = MoveForward(modelEye, rotation, -0.2f);
		}

		if (keys[SDL_SCANCODE_A]) rotation -= 5.0f;
		if (keys[SDL_SCANCODE_D]) rotation += 5.0f;
		break;
	default:
		break;
	}

}