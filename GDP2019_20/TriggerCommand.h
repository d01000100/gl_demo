#pragma once
#include "iCommand.h"

#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <queue>

#include "GameItemFactory/iGameItem.h"

class TriggerCommand : public iCommand
{
private:
	aGameItem* checkItem;
	glm::vec3 mins, maxs;
	bool isTriggered;
	float lifeTime, timer;
	iCommand* toTrigger;
public:
	TriggerCommand(
		aGameItem* gameItem,
		glm::vec3 mins,
		glm::vec3 maxs,
		iCommand* toTrigger,
		float lifeTime)
	{
		// TODO: manage more than 180° turn rotation
		this->checkItem = gameItem;
		this->mins = mins;
		this->maxs = maxs;
		this->toTrigger = toTrigger;
		isTriggered = false;
		this->lifeTime = lifeTime;
		this->timer = 0;
	}

	virtual bool isDone() {
		return isTriggered ? toTrigger->isDone() : timer > lifeTime;
	}

	virtual void update(float deltaTime)
	{
		if (!isDone())
		{
			if (!isTriggered)
			{
				glm::vec3 pos = checkItem->getPos();
				isTriggered = mins.x < pos.x && mins.y < pos.y && mins.z < pos.z &&
					pos.x < maxs.x && pos.y < maxs.y && pos.z < maxs.z;
			}
			else
			{
				toTrigger->update(deltaTime);
			}
			timer += deltaTime;
		}
	}
};