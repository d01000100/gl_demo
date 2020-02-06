#pragma once
#include "iCommand.h"

#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <queue>
#include <algorithm>

#include "GameItemFactory/iGameItem.h"
#include "quaternions_utils.h"

class LookToward : public iCommand
{
private:
	glm::quat startOrientation, endOrientation;
	std::queue<glm::quat> orientations;
	float totalDuration, currentTime;
	aGameItem* gameItem;
	bool isInitialized;
public:
	LookToward(
		aGameItem* gameItem,
		glm::vec3 direction,
		float totalDuration)
	{

		this->gameItem = gameItem;
		this->totalDuration = totalDuration;
		this->endOrientation = RotationBetweenVectors(gameItem->getBaseDirection(), glm::normalize(direction));

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