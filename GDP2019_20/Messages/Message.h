#pragma once

#include <string>
#include <glm/vec3.hpp>

struct sMessage {
	std::string name;
	std::string sValue;
	float fValue;
	int iValue;
	glm::vec3 v3Value;
};