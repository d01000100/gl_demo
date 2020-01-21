#include "Steerings.h"
#include <glm/glm.hpp>

glm::vec3 Steerings::seek(glm::vec3 pos, glm::vec3 vel, glm::vec3 target, float maxVel)
{
	glm::vec3 desiredVelocity = glm::normalize(target - pos) * maxVel;
	float distance = glm::distance(target, pos);
	glm::vec3 steeringVel = desiredVelocity - vel;
	return steeringVel;
}
