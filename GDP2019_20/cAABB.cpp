#include "cAABB.h"

#include "util.h"
#include "globalStuff.h"
#include "colors.h"

cAABB::cAABB(glm::vec3 min, float size)
{
	this->mins = min;
	this->size = size;
}

cAABB::cAABB(glm::vec3 mins) {
	this->mins = mins;
	this->size = 100.0f; 
	// por el modelo de battlestar galactica
}

glm::vec3 cAABB::getMins() { return mins; }
glm::vec3 cAABB::getMaxs() { return mins + size; }
glm::vec3 cAABB::getCenter() { return mins + (size / 2); }
float cAABB::getSize() { return size; }

void drawTriangle(sNiceTriangle* triangle)
{
	::g_pDebugRenderer->addLine(triangle->a, triangle->b, Colors::red, 0.01f);
	::g_pDebugRenderer->addLine(triangle->a, triangle->c, Colors::red, 0.01f);
	::g_pDebugRenderer->addLine(triangle->c, triangle->b, Colors::red, 0.01f);
}

void cAABB::Draw()
{
	glm::vec3 maxs,
		minXmaxs, minYmaxs, minZmaxs,
		maxXmins, maxYmins, maxZmins;

	maxs = getMaxs();

	minXmaxs = glm::vec3(mins.x, maxs.y, maxs.z);
	minYmaxs = glm::vec3(maxs.x, mins.y, maxs.z);
	minZmaxs = glm::vec3(maxs.x, maxs.y, mins.z);

	maxXmins = glm::vec3(maxs.x, mins.y, mins.z);
	maxYmins = glm::vec3(mins.x, maxs.y, mins.z);
	maxZmins = glm::vec3(mins.x, mins.y, maxs.z);

	// edges that touch mins
	::g_pDebugRenderer->addLine(mins, maxXmins, Colors::white, 0.01f);
	::g_pDebugRenderer->addLine(mins, maxYmins, Colors::white, 0.01f);
	::g_pDebugRenderer->addLine(mins, maxZmins, Colors::white, 0.01f);

	// edges that touch maxs
	::g_pDebugRenderer->addLine(maxs, minXmaxs, Colors::white, 0.01f);
	::g_pDebugRenderer->addLine(maxs, minYmaxs, Colors::white, 0.01f);
	::g_pDebugRenderer->addLine(maxs, minZmaxs, Colors::white, 0.01f);

	// other edges
	::g_pDebugRenderer->addLine(maxYmins, minXmaxs, Colors::white, 0.01f);
	::g_pDebugRenderer->addLine(maxYmins, minZmaxs, Colors::white, 0.01f);
	::g_pDebugRenderer->addLine(maxZmins, minXmaxs, Colors::white, 0.01f);
	::g_pDebugRenderer->addLine(maxZmins, minYmaxs, Colors::white, 0.01f);
	::g_pDebugRenderer->addLine(maxXmins, minYmaxs, Colors::white, 0.01f);
	::g_pDebugRenderer->addLine(maxXmins, minZmaxs, Colors::white, 0.01f);

	for (std::set<sNiceTriangle*>::iterator t = triangles.begin();
		t != triangles.end(); t++) {

		//drawTriangle(*t);
	}
}