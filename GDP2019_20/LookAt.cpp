#include "LookAt.h"

LookAtCommand::LookAtCommand(
	aGameItem* gameItem,
	aGameItem* targetItem,
	float totalDuration)
{
	this->gameItem = gameItem;
	this->targetItem = targetItem;
	this->totalDuration = totalDuration;

	currentTime = 0.0f;
	isInitialized = false;
}

bool LookAtCommand::isDone() {
	return currentTime > totalDuration;
}

void LookAtCommand::update(float deltaTime)
{
	if (!isInitialized)
	{
		isInitialized = true;
	}

	if (!isDone())
	{
		glm::vec3 currentDirection = glm::normalize(targetItem->getPos() - gameItem->getPos());
		gameItem->setDirection(currentDirection);

		this->currentTime += deltaTime;
	}
}