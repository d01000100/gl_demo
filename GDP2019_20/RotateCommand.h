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
	float totalDuration, currentTime,
		easeInDuration, easeOutDuration,
		easeOutStart,
		t, maxVel;
	aGameItem* gameItem;
	bool isInitialized;
public:
	RotateCommand(
		aGameItem* gameItem,
		glm::vec3 deltaEulerRotation,
		float totalDuration,
		float easeInDuration = 0,
		float easeOutDuration = 0)
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

		this->easeInDuration = easeInDuration;
		this->easeOutDuration = easeOutDuration;
		t = 0;
		this->easeOutStart = totalDuration - easeOutDuration;

		float maxVelDuration = totalDuration - easeInDuration - easeOutDuration;
		maxVelDuration = std::max(0.0001f, maxVelDuration);
		// This was NOT trivial
		this->maxVel = 1.0f / (maxVelDuration + easeInDuration / 2 + easeOutDuration / 2);

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
			float currentVel = 0;
			if (currentTime < easeInDuration) // ease in
			{
				currentVel = maxVel * glm::smoothstep(0.0f, easeInDuration, currentTime);
			}
			else if (currentTime < easeOutStart) // just move
			{
				currentVel = maxVel;
			}
			else // ease Out
			{
				currentVel = maxVel * glm::smoothstep(totalDuration, easeOutStart, currentTime);
			}
			t += currentVel * deltaTime;

			glm::quat qCurrentRotation
				= glm::slerp(startOrientation, this->endOrientation,
					t);

			gameItem->setOrientation(qCurrentRotation);
			//gameItem->addOrientation(eulerRotationVel * deltaTime);

			this->currentTime += deltaTime;
		}
	}
};