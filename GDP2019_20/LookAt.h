#pragma once
#include "iCommand.h"

#include <glm/gtc/quaternion.hpp>
#include <algorithm>

#include "GameItemFactory/iGameItem.h"
#include "quaternions_utils.h"

class LookAtCommand : public iCommand
{
private:
	float totalDuration, currentTime, a;
	aGameItem* gameItem, *targetItem;
	bool isInitialized;
public:
	LookAtCommand(aGameItem* gameItem,
		aGameItem* targetItem,
		float totalDuration);
	virtual bool isDone();
	virtual void update(float deltaTime);
};