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
};