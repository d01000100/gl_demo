#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <fstream>

#include "GFLW_callbacks.h"
#include "globalStuff.h"			// for find object
#include "Camera.h"
#include "FollowCamera.h"
#include "Scene.h"
#include "SceneEditor.h"
#include "JSON_IO.h"
#include "cGameObject.h"

#include <stdio.h>		// for fprintf()

bool isShiftKeyDownByAlone(int mods);
bool isCtrlKeyDownByAlone(int mods);

const float CAMERAROTATIONSPEED = 0.05f; // glm::radians(0.1f);
const float CAMERAZOOMSPEED = 10.0f;

const float TRANSLATION_STEP = 0.5f;
const float ROTATION_STEP = glm::radians(0.5f);

Camera* theCamera = Camera::getTheCamera();
Scene* theScene = Scene::getTheScene();
SceneEditor* theEditor = SceneEditor::getTheEditor();

void camera_translate(int key, int action, int mods) {
	if (!isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods)) {
		if (key == GLFW_KEY_A)
		{
			theCamera->translate(glm::vec3(0.0f, 0.0f, 1.0f));
		}
		if (key == GLFW_KEY_D)
		{
			theCamera->translate(glm::vec3(0.0f, 0.0f, -1.0f));
		}
		if (key == GLFW_KEY_W)
		{
			theCamera->translate(glm::vec3(1.0f, 0.0f, 0.0f));
		}
		if (key == GLFW_KEY_S)
		{
			theCamera->translate(glm::vec3(-1.0f, 0.0f, 0.0f));
		}
	}
}

void camera_orbit(int key, int action, int mods) {
	if (!isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods))
	{
		if (key == GLFW_KEY_A)
		{
			theCamera->moveLeft(CAMERAROTATIONSPEED);
		}
		if (key == GLFW_KEY_D)
		{
			theCamera->moveRight(CAMERAROTATIONSPEED);
		}
		if (key == GLFW_KEY_Q)
		{
			theCamera->zoom(CAMERAZOOMSPEED);
		}
		if (key == GLFW_KEY_E)
		{
			theCamera->zoom(-CAMERAZOOMSPEED);
		}
		if (key == GLFW_KEY_W)
		{
			theCamera->moveUp(CAMERAROTATIONSPEED);
		}
		if (key == GLFW_KEY_S)
		{
			theCamera->moveDown(CAMERAROTATIONSPEED);
		}
	}
}

void thrusterControls(int key, int action, int mods)
{
	aGameItem* player = theScene->findItem("player");
	if (player && isShiftKeyDownByAlone(mods)) {
		sMessage messageToObject; messageToObject.name = "apply velocity";
		float playerAccel = 1.0f;
		switch (key)
		{
		case GLFW_KEY_W:
			messageToObject.v3Value = glm::vec3(0, playerAccel, 0);
			player->recieveMessage(messageToObject);
			break;
		case GLFW_KEY_S:
			messageToObject.v3Value = glm::vec3(0, -playerAccel, 0);
			player->recieveMessage(messageToObject);
			break;
		case GLFW_KEY_A:
			messageToObject.v3Value = glm::vec3(playerAccel, 0, 0);
			player->recieveMessage(messageToObject);
			break;
		case GLFW_KEY_D:
			messageToObject.v3Value = glm::vec3(-playerAccel, 0, 0);
			player->recieveMessage(messageToObject);
			break;
		case GLFW_KEY_Q:
			messageToObject.v3Value = glm::vec3(0, 0, -playerAccel);
			player->recieveMessage(messageToObject);
			break;
		case GLFW_KEY_E:
			messageToObject.v3Value = glm::vec3(0, 0, playerAccel);
			player->recieveMessage(messageToObject);
			break;
		case GLFW_KEY_SPACE:
			messageToObject.name = "stop";
			player->recieveMessage(messageToObject);
			break;
		}
	}
}

void velocityControls(std::string player_name, GLFWwindow* window)
{
	cGameObject* player = (cGameObject*)theScene->findItem(player_name);
	float playerSpeed = 20.0f;
	float pitchSpeed = 2.0f;
	if (player && player->physics)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			player->physics->velocity = player->getDirection() * playerSpeed;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			player->physics->velocity = player->getDirection() * -playerSpeed;
		if (glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS)
			player->physics->velocity = glm::vec3(0);
		
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			player->yaw(pitchSpeed);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			player->yaw(-pitchSpeed);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//camera_orbit(key, action, mods);
	//thrusterControls(key, action, mods);
	if ( !isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods) )
	{		
		// save camera
		if (key == GLFW_KEY_C && action == GLFW_PRESS) {
			theScene->storeCurrentCamera();
		}

		//if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
		//	theScene->changeCamera();
		//}
		//if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		//	theScene->setCamera("skull cam");
		//}
		//if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		//	theScene->setCamera("cave cam");
		//}
		//if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		//	theScene->setCamera("all scene");
		//}

		// to sceneEditor
		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			sMessage message; 
			message.name = "simple press";
			message.sValue = "down";
			theEditor->recieveMessage(message);
		}

		if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			sMessage message;
			message.name = "simple press";
			message.sValue = "up";
			theEditor->recieveMessage(message);
		}

		if (key == GLFW_KEY_G && action == GLFW_PRESS)
		{
			::isDebug = !::isDebug;
		}

		if (key == GLFW_KEY_PAUSE && action == GLFW_PRESS)
		{
			::isRunning = !::isRunning;
		}

		// Individual obj mode
		if (key == GLFW_KEY_T && action == GLFW_PRESS) {
			sMessage message;
			message.name = "simple press";
			message.sValue = "t";
			theEditor->recieveMessage(message);
		}

		if (key == GLFW_KEY_R && action == GLFW_PRESS) {
			sMessage message;
			message.name = "simple press";
			message.sValue = "r";
			theEditor->recieveMessage(message);
		}

		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			sMessage message;
			message.name = "simple press";
			message.sValue = "z";
			theEditor->recieveMessage(message);
		}

		if (key == GLFW_KEY_V && action == GLFW_PRESS) {
			sMessage message;
			message.name = "simple press";
			message.sValue = "v";
			theEditor->recieveMessage(message);
		}

		if (key == GLFW_KEY_SPACE && 
			action == GLFW_PRESS) {
			sMessage message;
			message.name = "simple press";
			message.sValue = "space";
			theEditor->recieveMessage(message);
		}

		// General edition mode
		if (key == GLFW_KEY_O && action == GLFW_PRESS) {
			sMessage message;
			message.name = "simple press";
			message.sValue = "o";
			theEditor->recieveMessage(message);
		}
		if (key == GLFW_KEY_L && action == GLFW_PRESS) {
			sMessage message;
			message.name = "simple press";
			message.sValue = "l";
			theEditor->recieveMessage(message);
		}

		if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
			saveScene(theScene, ::scene_filename);
		}

		if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
			theScene->reloadScene(::scene_filename);
			theEditor->init(theScene);
		}

		if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
			sMessage message;
			message.name = "simple press";
			message.sValue = "f1";
			theEditor->recieveMessage(message);
		}

		// dsp controls
		if ((key >= GLFW_KEY_1 && key <= GLFW_KEY_9) 
			&& action == GLFW_PRESS) 
		{
			sMessage message;
			message.name = "number";
			message.iValue = key - GLFW_KEY_0;
			theEditor->recieveMessage(message);
		}
	}

	// Edit mode
	if (isShiftKeyDownByAlone(mods))
	{
		if (action != GLFW_RELEASE)
		{
			if (key == GLFW_KEY_A)
			{
				sMessage message;
				message.name = "press with shift";
				message.sValue = "a";
				theEditor->recieveMessage(message);
			}
			if (key == GLFW_KEY_D)
			{
				sMessage message;
				message.name = "press with shift";
				message.sValue = "d";
				theEditor->recieveMessage(message);
			}
			if (key == GLFW_KEY_Q)
			{
				sMessage message;
				message.name = "press with shift";
				message.sValue = "q";
				theEditor->recieveMessage(message);
			}
			if (key == GLFW_KEY_E)
			{
				sMessage message;
				message.name = "press with shift";
				message.sValue = "e";
				theEditor->recieveMessage(message);
			}
			if (key == GLFW_KEY_W)
			{
				sMessage message;
				message.name = "press with shift";
				message.sValue = "w";
				theEditor->recieveMessage(message);
			}
			if (key == GLFW_KEY_S)
			{
				sMessage message;
				message.name = "press with shift";
				message.sValue = "s";
				theEditor->recieveMessage(message);
			}

			// audio controls
			if (key == GLFW_KEY_LEFT)
			{
				sMessage message;
				message.name = "press with shift";
				message.sValue = "left";
				theEditor->recieveMessage(message);
			}
			if (key == GLFW_KEY_UP)
			{
				sMessage message;
				message.name = "press with shift";
				message.sValue = "up";
				theEditor->recieveMessage(message);
			}
			if (key == GLFW_KEY_RIGHT)
			{
				sMessage message;
				message.name = "press with shift";
				message.sValue = "right";
				theEditor->recieveMessage(message);
			}
			if (key == GLFW_KEY_DOWN)
			{
				sMessage message;
				message.name = "press with shift";
				message.sValue = "down";
				theEditor->recieveMessage(message);
			}
		}
	}//if (isShiftKeyDownByAlone(mods))


	// Moving the pirate ship in a certain direction
	if (isCtrlKeyDownByAlone(mods))
	{
		
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

bool isShiftKeyDownByAlone(int mods)
{
	return mods == GLFW_MOD_SHIFT;
}

bool isCtrlKeyDownByAlone(int mods)
{
	return mods == GLFW_MOD_CONTROL;
}