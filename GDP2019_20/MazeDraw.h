#pragma once
#include "cGameObject.h"

struct MazeDraw
{
	static cGameObject* cube;
	static void addToScene(cGameObject* cube);
	static glm::vec3 coordToWorldSpace(int i, int j);
};
