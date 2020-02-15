#include "Flocking.h"
#include "Steerings.h"

bool Flocking::isNeighbor(cGameObject* vehicle, cGameObject* other, float radius)
{
	return vehicle != other && 
		glm::distance(vehicle->position, other->position) < radius;
}

std::vector<cGameObject*> Flocking::getNeighbors(cGameObject* vehicle, float radius)
{
	std::vector<cGameObject*> res;

	for (auto v : birbs)
	{
		if (isNeighbor(vehicle, v, radius))
			res.push_back(v);
	}
	return res;
}

void Flocking::update(float deltaTime)
{
	float totalPower = separationPower + cohesionPower + alignmentPower;
	float separationC = separationPower / totalPower;
	float alignmentC = alignmentPower / totalPower;
	float cohesionC = cohesionPower / totalPower;
	for (auto birb : birbs)
	{
		vObjs neighbors = getNeighbors(birb, neighborDistance);
		vObjs separationNeighbors = getNeighbors(birb, neighborDistance * 0.7);
		if (neighbors.size() > 0)
		{
			glm::vec3 separationVel = separationC * separationForce(separationNeighbors, birb);
			glm::vec3 cohesionVel = cohesionC * cohesionForce(neighbors, birb);
			glm::vec3 alignmentVel = alignmentC * alignmentForce(neighbors, birb);

			birb->physics->velocity += (separationVel + cohesionVel + alignmentVel) * deltaTime;

			if (glm::length(birb->physics->velocity) > maxVel)
				birb->physics->velocity = glm::normalize(birb->physics->velocity) * maxVel;

			birb->setDirection(birb->physics->velocity);
		}
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
	glm::vec3 averageVel(0);
	for (auto n : neighbors)
	{
		averageVel += n->physics->velocity / (float)neighbors.size();
	}
	if (glm::length( averageVel ) > 0)
		averageVel = glm::normalize(averageVel) * maxVel;
	return averageVel;
}

glm::vec3 Flocking::cohesionForce(vObjs neighbors, cGameObject* vehicle)
{
	glm::vec3 centerOfMass = glm::vec3(0);
	for (auto n : neighbors)
	{
		centerOfMass += n->position;
	}
	centerOfMass /= neighbors.size();
	return Steerings::seek(
		vehicle->position, 
		vehicle->physics->velocity,
		centerOfMass,
		maxVel
	);
}
