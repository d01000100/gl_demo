#include "Flocking.h"
#include "Steerings.h"

bool Flocking::isNeighbor(cGameObject* vehicle, cGameObject* other)
{
	return vehicle != other && 
		glm::distance(vehicle->position, other->position) < neighborDistance;
}

std::vector<cGameObject*> Flocking::getNeighbors(cGameObject* vehicle)
{
	std::vector<cGameObject*> res;

	for (auto v : birbs)
	{
		if (isNeighbor(vehicle, v))
			res.push_back(v);
	}
	return res;
}

void Flocking::update(float deltaTime)
{
	for (auto birb : birbs)
	{
		vObjs neighbors = getNeighbors(birb);
		glm::vec3 separationVel = separationForce(neighbors, birb);

		birb->physics->velocity += separationVel * deltaTime;
		
		if (glm::length(birb->physics->velocity) > maxVel)
			birb->physics->velocity = glm::normalize(birb->physics->velocity) * maxVel;

		birb->setDirection(birb->physics->velocity);
	}
}

glm::vec3 Flocking::separationForce(vObjs neighbors, cGameObject* vehicle)
{
	int neighborCount = neighbors.size();
	if (neighborCount > 0)
	{
		glm::vec3 fleeForce = glm::vec3(0);
		for (auto other : neighbors)
		{
			fleeForce += (Steerings::flee(
				vehicle->getPos(),
				vehicle->physics->velocity,
				other->getPos(),
				maxVel
			) / glm::distance(other->getPos(), vehicle->getPos()));
		}
		fleeForce /= neighborCount;
		fleeForce = glm::normalize(fleeForce) * maxVel;
		return fleeForce;
	} else
		return glm::vec3(0);
}

glm::vec3 Flocking::alignmentForce(vObjs neighbors, cGameObject* vehicle)
{
	return glm::vec3(0, 0, 0);
}

glm::vec3 Flocking::cohesionForce(vObjs neighbors, cGameObject* vehicle)
{
	return glm::vec3(0);
}
