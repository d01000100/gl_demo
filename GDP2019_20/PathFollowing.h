#pragma once
#include "cGameObject.h"

struct PathFollowing
{
	std::vector<cGameObject*> path;
	int currentNode = 0;
	bool isForward = true;

	/*
	 * Needs the scene to be initialized
	 */
	void init();
	void nextNode();

	void followPath(cGameObject* vehicle, float deltaTime);

	void inversePath();
};

