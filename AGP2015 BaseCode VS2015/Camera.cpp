#include "Camera.h"
#include "PlayableCharacter.h"

#define DEG_TO_RAD 0.017453293

/* Updates camera position. Same function used in character class. */
glm::vec3 Camera::MoveForward(glm::vec3 cam, GLfloat angle, GLfloat d) {
	return glm::vec3(cam.x + d*std::sin(angle*DEG_TO_RAD),
		cam.y, cam.z - d*std::cos(angle*DEG_TO_RAD));
}

glm::vec3 Camera::MoveRight(glm::vec3 pos, GLfloat angle, GLfloat d)
{
	return glm::vec3(pos.x + d*std::cos(angle*DEG_TO_RAD), pos.y, pos.z + d*std::sin(angle*DEG_TO_RAD));
}

/*The camera information is not loaded from a file, instead hardcoded.*/
void Camera::InitalStats()
{
	position = { 0.0f, 2.5f, 10.0f };
	at = { 0.0f, 1.0f, -6.0 };
	up = { 0.0f, 1.0f, 0.0f };

	FileLoader* fileLoader = new FileLoader;
	 //Initialize default output device 
	if (!BASS_Init(-1, 44100, 0, 0, NULL))
		std::cout << "Can't initialize device";

	songs = new HSAMPLE[4];
	//songs[0] = fileLoader->loadSample("Sound/Music/Battle_of_the_Titans.wav");
	delete fileLoader;
}

void Camera::Sound(int soundFile)
{
	HCHANNEL ch = BASS_SampleGetChannel(songs[soundFile], FALSE);
	BASS_ChannelSetAttribute(ch, BASS_ATTRIB_FREQ, 0);
	BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, 0.5);
	BASS_ChannelSetAttribute(ch, BASS_ATTRIB_PAN, -1);
	if (!BASS_ChannelPlay(ch, FALSE))
		std::cout << "Can't play sample" << std::endl;

}

void Camera::TranslateTo(float &currentPosition, float &newPosition)
{
	if (newPosition < currentPosition)
		currentPosition -= -0.01f;
	else if (newPosition > currentPosition)
		currentPosition += 0.01f;
}

/* Handles draw method for camera. Camera is always looking directly behind the player. */
void Camera::draw(glm::mat4 &object, glm::vec3 modelEye)
{
	switch (camera_Type)
	{
	case 1:
		at = MoveForward(modelEye, 0, 1.0f);
		break;
	case 2:

		TranslateTo(position.x, Cinematic_X);
		TranslateTo(position.y, Cinematic_Y);
		TranslateTo(position.z, Cinematic_Z);
		at = modelEye; // MoveForward(position, 0, 1.0f);
		break;
	case 3:
		at = MoveForward(position, 0, 1.0f);
		object = glm::lookAt(position, at, up);
		return;
		break;
	default:
		at = MoveForward(position, 0, 1.0f);
		break;
	}
	object = glm::lookAt(position, at, up);
	object = glm::translate(object, modelEye);
	object = glm::rotate(object, float((rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	object = glm::translate(object, -modelEye);

	//object = glm::lookAt(position, at, up);

	//object = glm::translate(object, modelEye);
	//object = glm::rotate(object, float((rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	//object = glm::translate(object, -modelEye);
}

void Camera::CinematicValues(glm::vec3 characterPosition, float playerRotation)
{
	int xMovement = rand() % 5 + 2;
	int yMovement = rand() % 4 + 1;
	int zMovement = rand() % 5 + 2;
	int xEnding = 0;
	int yEnding = 0;
	int zEnding = 0;
	int x_small_Modifier = 1;
	int y_small_Modifier = 3;
	int z_small_Modifier = 1;
	bool hasEffect = false;
	if (rand() % 2 == 0)
	{
		xMovement = -xMovement;
		x_small_Modifier = -1;
	}
	if (rand() % 2 == 0)
	{
		zMovement = -zMovement;
		z_small_Modifier = -1;
	}
	if (yMovement >= 4)
		y_small_Modifier = 0;

	//X - Position Movement
	if (rand() % 4 != 0)
	{
		hasEffect = true;
		if (rand() % 2 == 0)
			xEnding = rand() % xMovement + x_small_Modifier;
		else
			xEnding = rand() % 3 + xMovement;
	} else {
		xEnding = xMovement;
	}

	//Y - Position Movement
	if (rand() % 4 != 0)
	{
		hasEffect = true;
		if (rand() % 2 == 0)
			yEnding = rand() % yMovement + y_small_Modifier;
		else
			yEnding = rand() % 3 + yMovement;
	}
	else {
		yEnding = yMovement;
	}

	//Z - Position Movement
	if (rand() % 4 != 0 || hasEffect == false)
	{
		if (rand() % 2 == 0)
			zMovement = rand() % zMovement + z_small_Modifier;
		else
			zMovement = rand() % 3 + zMovement;
	} else {
		zEnding = zMovement;
	}

	position = glm::vec3(xMovement, yMovement, zMovement);
	Cinematic_X = xEnding;
	Cinematic_Y = yEnding;
	Cinematic_Z = zEnding;
	start = duration;
	Cinematic_Timer = rand() % 7 + 1;
}

void Camera::CombatCinematic(glm::mat4 &object, glm::vec3 modelEye)
{
	object = glm::lookAt(position, at, up);
	object = glm::translate(object, modelEye);
	object = glm::rotate(object, float((rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
	object = glm::translate(object, -modelEye);
}

/* Update the camera position to make it stay behind the player model at all times. */
void Camera::update(glm::vec3 modelEye, float playerRotation)
{
	duration = (((std::clock() - start) / (double)CLOCKS_PER_SEC));
	timeDifference = duration - start;
//	cout << duration << " cinematic timer: " << Cinematic_Timer << " start: " << start << " y timeDifference: " << timeDifference << endl;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_F12])
	{
		camera_Type++;
		if (camera_Type > 3)
			camera_Type = 1;

		SwitchState(camera_Type, NULL);
		cout << endl << endl << camera_Type << endl << endl << endl;
	}
	

	switch (camera_Type)
	{
	case 1:
		position.x = modelEye.x;
		position.y = modelEye.y + 1.5f;
		position.z = modelEye.z + 4.0f;
		rotation = playerRotation;
		break;
	case 2:
		if (timeDifference >= Cinematic_Timer || position.y < 0)
			CinematicValues(modelEye, playerRotation);
		break;
	case 3:
		if (keys[SDL_SCANCODE_W]) position = MoveForward(position, rotation, 0.1f);
		if (keys[SDL_SCANCODE_S]) position = MoveForward(position, rotation, -0.1f);
		if (keys[SDL_SCANCODE_A]) rotation -= 1.0f;
		if (keys[SDL_SCANCODE_D]) rotation += 1.0f;
		if (keys[SDL_SCANCODE_R]) position.y += 0.1f;
		if (keys[SDL_SCANCODE_F]) position.y -= 0.1f;
		break;
	default:
		break;
	}

}

void Camera::SwitchState(int state, PlayableCharacter* character)
{
	switch (state)
	{
	case THIRD_PERSON:

		break;
	case COMBAT_CINEMATIC:
		//Sound(0);// character->characterState);
		break;
	default:
		return;
		break;
	}
	camera_Type = state;
}