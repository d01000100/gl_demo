#pragma once
#include "iCommand.h"

#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <queue>

#include "GameItemFactory/iGameItem.h"

class RotateCommand : public iCommand
{
private:
	glm::quat startOrientation, endOrientation,
		deltaQRotation, nullQRotation;
	std::queue<glm::quat> orientations;
	glm::vec3 deltaEulerRotation,
		eulerRotationVel,
		currentEulerRotation;
	float totalDuration, currentTime;
	aGameItem* gameItem;
	bool isInitialized;
public:
	RotateCommand(
		aGameItem* gameItem,
		glm::vec3 deltaEulerRotation,
		float totalDuration)
	{
		// TODO: manage more than 180° turn rotation
		this->gameItem = gameItem;
		this->totalDuration = totalDuration;
		this->deltaEulerRotation = deltaEulerRotation;
		this->currentEulerRotation = deltaEulerRotation;
		// This makes wierd stuff
		this->eulerRotationVel = deltaEulerRotation / totalDuration;
		this->nullQRotation = glm::quat(glm::vec3(0, 0, 0));
		this->deltaQRotation = glm::quat(glm::radians(deltaEulerRotation));

		currentTime = 0.0f;
		isInitialized = false;
	}

	virtual bool isDone() {
		return currentTime > totalDuration;
	}

	virtual void update(float deltaTime)
	{
		if (!isInitialized)
		{
			// Quaterion based rotation
			this->startOrientation = gameItem->getOrientationQ();
			this->endOrientation = startOrientation * glm::quat(glm::radians(deltaEulerRotation));

			isInitialized = true;
		}

		if (!isDone())
		{
			// Quaterion based rotation
			float AmountOfRotationCompleted = currentTime / totalDuration;
			glm::quat qCurrentRotation
				= glm::slerp(startOrientation, this->endOrientation,
					AmountOfRotationCompleted);

			gameItem->setOrientation(qCurrentRotation);
			//gameItem->addOrientation(eulerRotationVel * deltaTime);

			this->currentTime += deltaTime;
		}
	}
};