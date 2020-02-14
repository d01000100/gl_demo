#pragma once
#include "cGameObject.h"

struct PathFollowing
{
	std::vector<glm::vec3> path = {
		glm::vec3(9.2, 0.0, 0.0),
		glm::vec3(27.8,0,	28.8),
		glm::vec3(-15, 0.0, 16.6),
		glm::vec3(25.6, 0.0, -24),
		glm::vec3(-24.8, 0.0, 0.0),
	};
	int currentNode = 0;
	bool isForward = true;

	void nextNode();

	void followPath(cGameObject* vehicle, float deltaTime);

	void inversePath();
};

