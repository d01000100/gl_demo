#pragma once
#include "iCommand.h"
#include "GameItemFactory/iGameItem.h"

#include <glm/vec3.hpp>

class FollowCommand : public iCommand
{
private:
	float totalDuration, currentTime,
		stopDistance, slowDownDistance,
		maxSpeed;
	glm::vec3 offset;
	aGameItem *me, *targetItem;
	bool isInitialized;
public:
	FollowCommand(
		aGameItem* gameItem,
		aGameItem* targetItem,
		glm::vec3 offset,
		float totalDuration,
		float maxSpeed,
		float speedDownDistance,
		float stopDistance)
	{
		this->me = gameItem;
		this->targetItem = targetItem;
		this->offset = offset;
		this->totalDuration = totalDuration;
		this->maxSpeed = maxSpeed;
		this->slowDownDistance = speedDownDistance;
		this->stopDistance = stopDistance;

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
			isInitialized = true;
		}

		if (!isDone())
		{
			glm::vec3 targetCurrentPos = targetItem->getPos() + offset;
			glm::vec3 travelDir = glm::normalize(targetCurrentPos - me->getPos());
			float missingDistance = glm::distance(targetCurrentPos, me->getPos());

			if (missingDistance < stopDistance) { return; }

			float currentSpeed = maxSpeed;

			if (missingDistance < this->slowDownDistance)
			{
				float speedRatio = glm::smoothstep(stopDistance, slowDownDistance, missingDistance);

				currentSpeed *= speedRatio;
			}

			glm::vec3 newPos = me->getPos() + (travelDir * currentSpeed * deltaTime);
			me->setPos(newPos);

			this->currentTime += deltaTime;
		}
	}
};