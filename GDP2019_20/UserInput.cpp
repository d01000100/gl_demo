#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <fstream>

#include "GFLW_callbacks.h"
#include "globalStuff.h"			// for find object
#include "Camera.h"
#include "Scene.h"
#include "SceneEditor.h"
#include "JSON_IO.h"

#include <stdio.h>		// for fprintf()

bool isShiftKeyDownByAlone(int mods);
bool isCtrlKeyDownByAlone(int mods);

const float CAMERAROTATIONSPEED = 0.05f; // glm::radians(0.1f);
const float CAMERAZOOMSPEED = 2.0f;

const float TRANSLATION_STEP = 0.5f;
const float ROTATION_STEP = glm::radians(0.5f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Camera* theCamera = Camera::getTheCamera();
	Scene* theScene = Scene::getTheScene();
	iMessagable* theEditor = SceneEditor::getTheEditor();

	if ( !isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods) )
	{
		if (key == GLFW_KEY_A)
		{
			//theCamera->moveLeft(CAMERAROTATIONSPEED);
			theCamera->translate(glm::vec3(0.0f, 0.0f, -1.0f));
		}
		if (key == GLFW_KEY_D)
		{
			//theCamera->moveRight(CAMERAROTATIONSPEED);
			theCamera->translate(glm::vec3(0.0f, 0.0f, 1.0f));
		}
		if (key == GLFW_KEY_Q)
		{
			//theCamera->zoom(CAMERAZOOMSPEED);
		}
		if (key == GLFW_KEY_E)
		{
			//theCamera->zoom(-CAMERAZOOMSPEED);
		}
		if (key == GLFW_KEY_W)
		{
			//theCamera->moveUp(CAMERAROTATIONSPEED);
			theCamera->translate(glm::vec3(1.0f, 0.0f, 0.0f));
		}
		if (key == GLFW_KEY_S)
		{
			//theCamera->moveDown(CAMERAROTATIONSPEED);
			theCamera->translate(glm::vec3(-1.0f, 0.0f, 0.0f));
		}

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