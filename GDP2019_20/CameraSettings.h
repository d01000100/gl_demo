#pragma once

#include <glm/vec3.hpp>
#include <string>

struct sCameraSettings {
	glm::vec3 position, target;
	std::string name;
};