#pragma once

#include <glm/vec3.hpp>
#include <set>

#include "cMesh.h"

class cAABB
{
private:
	glm::vec3 mins;
	float size;
public:
	std::set<sNiceTriangle*> triangles;
	cAABB(glm::vec3 min, float size);
	cAABB(glm::vec3 mins);
	glm::vec3 getMins();
	glm::vec3 getMaxs();
	glm::vec3 getCenter();
	float getSize();
	void Draw();
};

