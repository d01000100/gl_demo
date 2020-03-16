#pragma once
#include "Camera.h"
#include "cGameObject.h"

struct MouseControls
{
	static glm::vec2 cursorPos, cursorPosOld;
	static bool isCursorReady;
	static void updateCursorPos();
	static bool isCursorInScreen();
	static glm::vec2 getCursorDelta();
	static void mouseListen(Camera* camera, cGameObject* player = nullptr);
};
