#include "MoveCommand.h"

#include <algorithm>
#include <iostream>

MoveCommand::MoveCommand(
	iGameItem* gameItem, 
	glm::vec3 finalPos, 
	float totalTime, 
	float easeInDuration,
	float easeOutDuration)
{
	hasInitialized = false;

	this->gameItem = gameItem;
	this->finalPos = finalPos;
	this->totalTime = totalTime;
	this->spentTime = 0.0f;
	this->easeInEnd = easeInDuration;
	this->easeOutDuration = easeOutDuration;
	this->easeOutStart = totalTime - easeOutDuration;
}

void MoveCommand::init()
{
	if (!hasInitialized)
	{
		this->startPos = gameItem->getPos();
		this->distance = glm::distance(startPos, finalPos);
		float maxVelDuration = totalTime - easeInDuration - easeOutDuration;
		maxVelDuration = std::max(0.0001f, maxVelDuration);
		// This was NOT trivial
		this->maxVel = distance / (maxVelDuration + easeInDuration / 2 + easeOutDuration / 2);
		this->direction = glm::normalize(finalPos - startPos);

		hasInitialized = true;

		//std::cout << "startPos: " << glm::to_string(startPos) << std::endl
		//	<< ""
	}
}

bool MoveCommand::isDone()
{
	return spentTime >= totalTime;
}

void MoveCommand::update(float deltaTime)
{
	init();
	if (!isDone()) 
	{
		float currentVel;
		// calculate velocity
		if (spentTime < easeInEnd) // ease in
		{
			currentVel = maxVel * glm::smoothstep(0.0f, easeInEnd, spentTime);
		}
		else if (spentTime < easeOutStart) // just move
		{
			currentVel = maxVel;
		}
		else // ease Out
		{
			currentVel = maxVel * glm::smoothstep(totalTime, easeOutStart, spentTime);
		}

		// move object
		glm::vec3 newPos = gameItem->getPos() + (direction * currentVel * deltaTime);
		gameItem->setPos(newPos);

		spentTime += deltaTime;
	}
}