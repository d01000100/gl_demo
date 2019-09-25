#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "commonstuff.h"
#include <string>

class cRobot
{
public:

	glm::vec3 position;
	glm::vec3 velocity;

	float energy;

	void Update(void);

	void PrintState(void);

	std::string name;

	unsigned int getUniqueID(void);
private:
	unsigned int m_uniqueID;
};
