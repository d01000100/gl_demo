#pragma once
#include "cGameObject.h"

typedef std::vector<cGameObject*> vObjs;

struct Flocking
{
	std::vector<cGameObject*> birbs;
	float neighborDistance = 18.0f;
	float maxVel = 10.0f;
	float cohesionPower = 0.33,
		separationPower = 0.33,
		alignmentPower = 0.33;
	
	bool isNeighbor(cGameObject* vehicle, cGameObject* other, float radius);
	vObjs getNeighbors(cGameObject* vehicle, float radius);
	void update(float deltaTime);
	glm::vec3 separationForce(vObjs neighbors, cGameObject* vehicle);
	glm::vec3 alignmentForce(vObjs neighbors, cGameObject* vehicle);
	glm::vec3 cohesionForce(vObjs neighbors, cGameObject* vehicle);
};
