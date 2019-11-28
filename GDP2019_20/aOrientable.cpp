#include "aOrientable.h"

#include "globalStuff.h"

void aOrientable::debugOrientationAxis(glm::vec3 origin)
{
	::g_pDebugRenderer->addLine(origin, origin + getDirection() * 7.0f, Colors::red, 0.01f);
	::g_pDebugRenderer->addLine(origin, origin + getUp() * 5.0f, Colors::yellow, 0.01f);
	::g_pDebugRenderer->addLine(origin, origin + getRight() * 3.0f, Colors::blue, 0.01f);
}