#pragma once
#include "iCommand.h"
#include "GameItemFactory/iGameItem.h"
#include <glm/vec3.hpp>

class MoveCommand : public iCommand
{
protected:
	glm::vec3 startPos, finalPos, direction;
	float spentTime, totalTime,
		easeInDuration, easeOutDuration,
		easeInEnd, easeOutStart,
		distance,
		maxVel;
	aGameItem* gameItem;
	bool hasInitialized;

	void init();
public:
	MoveCommand(
		aGameItem* gameItem,
		glm::vec3 startPos,
		glm::vec3 finalPos, 
		float totalTime,
		float easeInDuration,
		float easeOutDuration);
	virtual bool isDone();
	virtual void update(float deltaTime);
};