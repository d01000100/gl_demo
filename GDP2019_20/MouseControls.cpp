#include "MouseControls.h"
#include "globalStuff.h"
#include "GLCommon.h"

glm::vec2 MouseControls::cursorPos;
glm::vec2 MouseControls::cursorPosOld;
bool MouseControls::isCursorReady = false;

void MouseControls::updateCursorPos()
{
	if (isCursorInScreen())
	{
		cursorPosOld = cursorPos;
		double x, y;
		glfwGetCursorPos(::window, &x, &y);
		cursorPos.x = x; cursorPos.y = y;
		isCursorReady = true;
	}
	else
	{
		MouseControls::cursorPosOld = MouseControls::cursorPos = glm::vec2(0);
		MouseControls::isCursorReady = false;
	}
}

bool MouseControls::isCursorInScreen()
{
	//return glfwGetWindowAttrib(::window, GLFW_HOVERED);
	return true;
}

glm::vec2 MouseControls::getCursorDelta()
{
	updateCursorPos();
	if (isCursorReady)
	{
		return cursorPos - cursorPosOld;
	}
	else
	{
		return glm::vec2(0);
	}
}

void MouseControls::mouseListen(Camera* camera, cGameObject* player)
{
	auto cursorDelta = getCursorDelta();
	float rotationVel = 0.01f;
	if (camera)
	{
		if (cursorDelta.y > 0)
		{
			camera->moveUp(rotationVel);
		}
		if (cursorDelta.y < 0)
		{
			camera->moveDown(rotationVel);
		}
		if (cursorDelta.x > 0)
		{
			camera->moveRight(rotationVel);
		}
		if (cursorDelta.x < 0)
		{
			camera->moveLeft(rotationVel);
		}
	}

	if (player)
	{
		player->yaw(cursorDelta.x);
		player->pitch(cursorDelta.y);
	}
}
