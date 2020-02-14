#include "PathFollowing.h"
#include "Steerings.h"
#include <iostream>

void PathFollowing::nextNode()
{
	std::cout << (isForward ? "Forward. " : "Backwards. ") << "Current Node: " << currentNode << std::endl;
	if (path.size() > 1)
	{
		if (isForward)
		{
			currentNode = (currentNode + 1) % path.size();
		}
		else
		{
			currentNode--;
			if (currentNode < 0)
			{
				currentNode = ((int)path.size()) - 1;
			}
		}
	}
	std::cout << "Next Node: " << currentNode << "\n\n";
}

void PathFollowing::followPath(cGameObject* vehicle, float deltaTime)
{
	if (path.size() > 0)
	{
		glm::vec3 target = path[currentNode];
		float distance = glm::distance(target, vehicle->getPos());
		float maxVel = 15.0f;
		glm::vec3 steeringVel = Steerings::seek(
			vehicle->getPos(),
			vehicle->physics->velocity,
			target,
			maxVel);

		vehicle->physics->velocity += steeringVel * deltaTime;
		if (glm::length(vehicle->physics->velocity) > maxVel)
			vehicle->physics->velocity = glm::normalize(vehicle->physics->velocity) * maxVel;
		vehicle->setDirection(vehicle->physics->velocity);

		if (distance < 5.f)
		{
			nextNode();
		}
	}
}

void PathFollowing::inversePath()
{
	isForward = !isForward;
	nextNode();
}
