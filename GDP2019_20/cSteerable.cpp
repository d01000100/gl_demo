#include "cSteerable.h"
#include "Steerings.h"

cSteerable::cSteerable(cGameObject *a) :
	maxVel(10.0f),
	agent(a)
{
}

void cSteerable::Seek(cGameObject* target, float deltaTime)
{
	glm::vec3 steeringVel = Steerings::seek(
		agent->getPos(),
		agent->physics->velocity,
		target->getPos(),
		maxVel);
	agent->physics->velocity += steeringVel * deltaTime;
}
