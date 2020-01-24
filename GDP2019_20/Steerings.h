#pragma once
#include <glm/vec3.hpp>
class Steerings
{
public:
	static glm::vec3 seek(
		glm::vec3 pos,
		glm::vec3 vel,
		glm::vec3 target,
		float maxVel);
	static glm::vec3 flee(
		glm::vec3 pos,
		glm::vec3 vel,
		glm::vec3 target,
		float maxVel);
	static glm::vec3 approach(
		glm::vec3 pos,
		glm::vec3 vel,
		glm::vec3 target,
		float maxVel,
		float slowingRadius,
		float stopRadius = 0.0f);
	static glm::vec3 wander(
		glm::vec3 pos, 
		glm::vec3 vel, 
		float maxVel,
		glm::vec3 direction,
		float distanceToPoint,
		float circleRadius);
	static glm::vec3 pursue(
		glm::vec3 pos, 
		glm::vec3 vel, 
		glm::vec3 target, 
		glm::vec3 targetVel, 
		float maxVel);
	static glm::vec3 evade(
		glm::vec3 pos,
		glm::vec3 vel,
		glm::vec3 target,
		glm::vec3 targetVel,
		float maxVel);
};