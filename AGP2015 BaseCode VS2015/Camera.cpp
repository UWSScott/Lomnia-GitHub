#include "Camera.h"
#include "PlayableCharacter.h"

#define DEG_TO_RAD 0.017453293
HCHANNEL combatCh;
HCHANNEL ch;

/* Updates camera position. Same function used in character class. */
glm::vec3 Camera::MoveForward(glm::vec3 cam, GLfloat angle, GLfloat d) {
	return glm::vec3(cam.x + d*std::sin(angle*DEG_TO_RAD), cam.y, cam.z - d*std::cos(angle*DEG_TO_RAD));
}

glm::vec3 Filming_Look(glm::vec3 cam, GLfloat angle, GLfloat d, float camera_Pitch) {
	return glm::vec3(cam.x + d*std::sin(angle*DEG_TO_RAD), cam.y - (d*std::sin(camera_Pitch*DEG_TO_RAD)), cam.z - d*std::cos(angle*DEG_TO_RAD));
}


glm::vec3 Camera::MoveRight(glm::vec3 pos, GLfloat angle, GLfloat d)
{
	return glm::vec3(pos.x + d*std::cos(angle*DEG_TO_RAD), pos.y, pos.z + d*std::sin(angle*DEG_TO_RAD));
}

void Camera::InitalStats()
{
	Collider = new Collisions();
	position = { 0.0f, 2.5f, 10.0f };
	at = { 0.0f, 1.0f, -6.0 };
	up = { 0.0f, 1.0f, 0.0f };

	FileLoader* fileLoader = new FileLoader;
	 //Initialize default output device 
	if (!BASS_Init(-1, 44100, 0, 0, NULL))
		std::cout << "Can't initialize device";

	songs = new HSAMPLE[5];
	songs[0] = fileLoader->loadSample("Sound/Music/Menu_Music.wav"); //Menu
	songs[1] = fileLoader->loadSample("Sound/Music/Ambient_Hub.wav");; //Ambient
	songs[2] = fileLoader->loadSample("Sound/Music/Combat_Music.wav"); //Combat
	songs[3] = fileLoader->loadSample("Sound/Music/Shop_Music.wav"); //Vendor
	songs[4] = fileLoader->loadSample("Sound/Music/Death_Music.wav"); //Death
	songs[5] = fileLoader->loadSample("Sound/Music/Maze_Music.wav"); //Maze
	delete fileLoader;
}


void Camera::Sound(HCHANNEL &channel, int soundFile)
{
	BASS_ChannelPause(channel);
	channel = BASS_SampleGetChannel(songs[soundFile], FALSE);

	//BASS_ChannelSetAttribute(ch, BASS_ATTRIB_FREQ, 0);
	//BASS_ChannelSetAttribute(ch, BASS_ATTRIB_VOL, 0.5);
	//BASS_ChannelSetAttribute(ch, BASS_ATTRIB_PAN, -1);
	if (!BASS_ChannelPlay(channel, FALSE))
		std::cout << "Can't play sample" << std::endl;


	//std::cout << "song int: " << soundFile << std::endl; 
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
	case 0:
		at = MoveForward(position, 0, 1.0f);
		break;
	case 1:
		at = MoveForward(modelEye, 0, 1.0f);
		break;
	case 2:

		TranslateTo(position.x, Cinematic_X);
		TranslateTo(position.y, Cinematic_Y);
		TranslateTo(position.z, Cinematic_Z);
		object = glm::lookAt(position, cinematicLookAt, up);
		object = glm::translate(object, cinematicLookAt);
		object = glm::rotate(object, float((rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
		object = glm::translate(object, -cinematicLookAt);
		return;
		break;
	case 3:
		at = Filming_Look(position, 0, 1.0f, camera_Pitch);
		object = glm::lookAt(position, at, up);
		object = glm::translate(object, position);
		object = glm::rotate(object, float((rotation)*DEG_TO_RAD), glm::vec3(0.0f, 1.0f, 0.0f));
		object = glm::translate(object, -position);
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
}

void Camera::CinematicValues(glm::vec3 characterPosition, float playerRotation)
{
	//rotation = playerRotation;
	cinematicLookAt = MoveForward(characterPosition, playerRotation, 2.0f);
	glm::vec3 combatPosition = characterPosition;// MoveForward(characterPosition, 0, 10.0f);
	//cout << "character position: X " << combatPosition.x << " Y " << combatPosition.y << " Z " << combatPosition.z << endl;
	int xMovement = rand() % 5 + 2;// +characterPosition.x;
	int yMovement = rand() % 4 + 1;// +characterPosition.y;
	int zMovement = rand() % 5 + 2;// +characterPosition.z;

								   //int xMovement = combatPosition.x;
								   //int yMovement = 0;
								   //int zMovement = combatPosition.z;
								   //yMovement += rand() % 2 + 1;

								   //if(rand() % 2 == 0)
								   //	xMovement += rand() % 3 + 2;
								   //else
								   //	xMovement -= rand() % 3 - 2;

								   //if (rand() % 2 == 0)
								   //	zMovement += rand() % 3 + 2;
								   //else
								   //	zMovement -= rand() % 3 - 2;

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
	if (rand() % 5 != 0)
	{
		hasEffect = true;
		if (rand() % 2 == 0 && xMovement != 0)
			xEnding = rand() % xMovement + x_small_Modifier;
		else
			xEnding = rand() % 3 + xMovement;
	}
	else {
		xEnding = xMovement;
	}

	//Y - Position Movement
	if (rand() % 5 != 0)
	{
		hasEffect = true;
		if (rand() % 2 == 0 && yMovement != 0)
			yEnding = rand() % yMovement + y_small_Modifier;
		else
			yEnding = rand() % 3 + yMovement;
	}
	else {
		yEnding = yMovement;
	}

	//Z - Position Movement
	if (rand() % 5 != 0 || hasEffect == false)
	{
		if (rand() % 2 == 0 && zMovement != 0)
			zEnding = rand() % zMovement + z_small_Modifier;
		else
			zEnding = rand() % 3 + zMovement;
	}
	else {
		zEnding = zMovement;
	}

	//if (xMovement < 0)
	//	xMovement += combatPosition.x * -1;
	//else
	//	xMovement += combatPosition.x;
	//if (yMovement < 0)
	//	yMovement += combatPosition.y * -1;
	//else
	//	yMovement += combatPosition.y;
	//if (zMovement < 0)
	//	zMovement += combatPosition.z * -1;
	//else
	//	zMovement += combatPosition.z;


	position = glm::vec3(combatPosition.x + xMovement, combatPosition.y + yMovement, combatPosition.z + zMovement);
	Cinematic_X = xEnding;
	Cinematic_Y = yEnding;
	Cinematic_Z = zEnding;
	start = duration;
	Cinematic_Timer = rand() % 7 + 1;
	//cout << "new position: " << position.x << " Y " << position.y << " Z " << position.z << endl;
	//cout << "cinematic end position: " << Cinematic_X << " Y " << Cinematic_Y << " Z " << Cinematic_Z << endl << endl;
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
	//cout << duration << " cinematic timer: " << Cinematic_Timer << " start: " << start << " y timeDifference: " << timeDifference << endl;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_P])
	{
		camera_Type++;
		if (camera_Type > 3)
			camera_Type = 0;

		SwitchState(camera_Type, NULL);
		cout << endl << endl << camera_Type << endl << endl << endl;
	}
	int xPos, yPos = 0;
	switch (camera_Type)
	{
	case FIRST_PERSON:
	//case 3:
		position.x = modelEye.x;
		position.y = modelEye.y + 1.5f;
		position.z = modelEye.z + -1.0f;
		rotation = playerRotation;
		break;
	case THIRD_PERSON:
	case 5:
		position.x = modelEye.x;
		position.y = modelEye.y + 1.5f;
		position.z = modelEye.z + 4.0f;
		rotation = playerRotation;
		break;
	case COMBAT_CINEMATIC:
	case DEATH_CINEMATIC:
		if (timeDifference >= Cinematic_Timer || position.y < 0)
			CinematicValues(modelEye, playerRotation);
		break;
	case FREE_VIEW:
		SDL_GetMouseState(&xPos, &yPos);
		if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_DOWN])
		{
			if (keys[SDL_SCANCODE_LEFT]) x_mouse_Smoothing -= 0.01f;
			if (keys[SDL_SCANCODE_RIGHT]) x_mouse_Smoothing += 0.01f;
			if (keys[SDL_SCANCODE_UP]) y_mouse_Smoothing += 0.01f;
			if (keys[SDL_SCANCODE_DOWN]) y_mouse_Smoothing -= 0.01f;
		} else {
			rotation = xPos * x_mouse_Smoothing;
			camera_Pitch = yPos * y_mouse_Smoothing;
			if (keys[SDL_SCANCODE_W]) position = MoveForward(position, rotation, 0.1f);
			if (keys[SDL_SCANCODE_S]) position = MoveForward(position, rotation, -0.1f);
			if (keys[SDL_SCANCODE_A]) position = MoveRight(position, rotation, -0.1f);
			if (keys[SDL_SCANCODE_D]) position = MoveRight(position, rotation, 0.1f);
			if (keys[SDL_SCANCODE_R]) position.y += 0.1f;
			if (keys[SDL_SCANCODE_F]) position.y -= 0.1f;
		}

		break;
	default:
		break;
	}

}

void Camera::SwitchState(int state, PlayableCharacter* character)
{
	//switch (state)
	//{
	//case FIRST_PERSON:

	//	break;
	//case THIRD_PERSON:

	//	break;
	//case COMBAT_CINEMATIC:
	//	//Sound(0);// character->characterState);
	//	break;
	//default:
	//	return;
	//	break;
	//}
	//camera_Type = state;
}

void Camera::SetPlayerStatus(int status, PlayableCharacter* character)
{
	if (status == STATE_DEATH && camera_Type == FREE_VIEW)
		return;

	int tempholder = camera_Type;
	if (camera_Type != status)
	{
		//if (camera_Type < 5)
		//{
			camera_Type = status;
		//}

		Sound(ch, status);
		if(status == STATE_DEATH)
			camera_Type = FREE_VIEW;

		
		//cout << "NEW CAMERA STATUS: " << status;
	}
}

int Camera::GetCameraType()
{
	return camera_Type;
}