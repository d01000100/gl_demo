#include "GLCommon.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "GFLW_callbacks.h"
#include "globalStuff.h"			// for find object
#include "Camera.h"

#include <stdio.h>		// for fprintf()

bool isShiftKeyDownByAlone(int mods);
bool isCtrlKeyDownByAlone(int mods);

Camera* theCamera = Camera::getTheCamera();
const float CAMERAROTATIONSPEED = 0.05f; // glm::radians(0.1f);
const float CAMERAZOOMSPEED = 2.0f;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ( !isShiftKeyDownByAlone(mods) && !isCtrlKeyDownByAlone(mods) )
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

	if (isShiftKeyDownByAlone(mods))
	{
		
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