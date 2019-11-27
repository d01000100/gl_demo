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
		t;
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
		// TODO: manage more than 180° turn rotation
		this->gameItem = gameItem;
		this->totalDuration = totalDuration;
		this->start = start;
		this->end = end;
		this->ctrl = ctrl;
		this->easeInDuration = easeInDuration;
		this->easeOutDuration = easeOutDuration;
		t = 0;
		this->easeOutStart = totalDuration - easeOutDuration;

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
			if (currentTime < easeInDuration)
			{
				printf("ease in curve...\n");
				float ratio = glm::smoothstep(0.0f, easeInDuration, currentTime);
				t = (currentTime / totalDuration) * ratio;
			}
			else if (currentTime < easeOutStart)
			{
				printf("normal curve...\n");
				t = currentTime / totalDuration;
			}
			else
			{
				printf("ease out curve...\n");
				float easeOutratio = glm::smoothstep(easeOutStart, totalDuration, currentTime);
				t = (easeOutStart / totalDuration) + (easeOutDuration / totalDuration) * easeOutratio;
			}
			printf("t: %f\n", t);
			// Beizer curve equation MAGIC
			glm::vec3 currentPos = (1 - t) * (1 - t) * start + 2 * (1 - t) * t * ctrl + t * t * end;

			gameItem->setPos(currentPos);

			currentTime += deltaTime;
		}
	}
};