#pragma once
#include "iCommand.h"

#include "GameItemFactory/iGameItem.h"

class RideCurve : public iCommand
{
private:
	glm::vec3 start, end, ctrl;
	float totalDuration, currentTime,
		easeInDuration, easeOutDuration,
		easeOutStart,
		t, currentVel, maxVel;
	aGameItem* gameItem;
	bool isInitialized;
public:
	RideCurve(
		aGameItem* gameItem,
		glm::vec3 start,
		glm::vec3 end,
		glm::vec3 ctrl,
		float totalDuration,
		float easeInDuration = 0,
		float easeOutDuration = 0)
	{
		this->gameItem = gameItem;
		this->totalDuration = totalDuration;
		this->start = start;
		this->end = end;
		this->ctrl = ctrl;
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
			gameItem->setPos(start);

			isInitialized = true;
		}

		if (!isDone())
		{
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
			// Beizer curve equation MAGIC
			glm::vec3 currentPos = (1 - t) * (1 - t) * start + 2 * (1 - t) * t * ctrl + t * t * end;

			gameItem->setPos(currentPos);

			currentTime += deltaTime;
		}
	}
};