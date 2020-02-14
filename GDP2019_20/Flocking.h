#pragma once
#include "cGameObject.h"

typedef std::vector<cGameObject*> vObjs;

struct Flocking
{
	std::vector<cGameObject*> birbs;
	float neighborDistance = 18.0f;
	float maxVel = 10.0f;
	
	bool isNeighbor(cGameObject* vehicle, cGameObject* other);
	vObjs getNeighbors(cGameObject* vehicle);
	void update(float deltaTime);
	glm::vec3 separationForce(vObjs neighbors, cGameObject* vehicle);
	glm::vec3 alignmentForce(vObjs neighbors, cGameObject* vehicle);
	glm::vec3 cohesionForce(vObjs neighbors, cGameObject* vehicle);
};
