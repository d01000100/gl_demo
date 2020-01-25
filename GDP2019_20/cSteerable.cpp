#include "cSteerable.h"
#include "Steerings.h"
#include <iostream>

cSteerable::cSteerable(cGameObject* a) :
	maxVel(10.0f),
	gameObject(a),
	approachDistance(30.0f),
	stopDistance(7.0f),
	wanderDistance(4.0f),
	wanderRadius(7.0f),
	evadePower(30.0f)
{
}

void cSteerable::Seek(cGameObject* target, float deltaTime)
{
	glm::vec3 steeringVel = Steerings::seek(
		gameObject->getPos(),
		gameObject->physics->velocity,
		target->getPos(),
		maxVel);
	gameObject->physics->velocity += steeringVel * deltaTime;
	if (glm::length(gameObject->physics->velocity) > maxVel)
		gameObject->physics->velocity = glm::normalize(gameObject->physics->velocity) * maxVel;
}

void cSteerable::Flee(cGameObject* target, float deltaTime)
{
	glm::vec3 steeringVel = Steerings::flee(
		gameObject->getPos(),
		gameObject->physics->velocity,
		target->getPos(),
		maxVel);
	gameObject->physics->velocity += steeringVel * deltaTime;
	if (glm::length(gameObject->physics->velocity) > maxVel)
		gameObject->physics->velocity = glm::normalize(gameObject->physics->velocity) * maxVel;
}

void cSteerable::Approach(cGameObject* target, float deltaTime)
{
	glm::vec3 steeringVel = Steerings::approach(
		gameObject->getPos(),
		gameObject->physics->velocity,
		target->getPos(),
		maxVel,
		approachDistance,
		stopDistance);
	gameObject->physics->velocity = steeringVel; // *deltaTime;
	if (glm::length(gameObject->physics->velocity) > maxVel)
		gameObject->physics->velocity = glm::normalize(gameObject->physics->velocity) * maxVel;
}

void cSteerable::Wander(float deltaTime)
{
	glm::vec3 steeringVel = Steerings::wander(
		gameObject->getPos(),
		gameObject->physics->velocity,
		maxVel,
		gameObject->getDirection(),
		wanderDistance,
		wanderRadius);
	gameObject->physics->velocity += steeringVel * deltaTime;
	gameObject->setDirection(gameObject->physics->velocity);
	if (glm::length(gameObject->physics->velocity) > maxVel)
		gameObject->physics->velocity = glm::normalize(gameObject->physics->velocity) * maxVel;
}

void cSteerable::Pursue(cGameObject* target, float deltaTime)
{
	glm::vec3 steeringVel = Steerings::pursue(
		gameObject->getPos(),
		gameObject->physics->velocity,
		target->getPos(),
		target->physics->velocity,
		maxVel);
	gameObject->physics->velocity += steeringVel * deltaTime;
	//if (glm::length(gameObject->physics->velocity) > maxVel)
	//	gameObject->physics->velocity = glm::normalize(gameObject->physics->velocity) * maxVel;
}

void cSteerable::Evade(cGameObject* target, float deltaTime)
{
	glm::vec3 steeringVel = Steerings::evade(
		gameObject->getPos(),
		gameObject->physics->velocity,
		target->getPos(),
		target->physics->velocity,
		maxVel);
	gameObject->physics->velocity += steeringVel * (deltaTime * evadePower);
	//if (glm::length(gameObject->physics->velocity) > maxVel)
	//	gameObject->physics->velocity = glm::normalize(gameObject->physics->velocity) * maxVel;
}