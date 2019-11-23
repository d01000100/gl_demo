#include "iGameItem.h"

#include <glm/gtx/string_cast.hpp>

#include "../quaternions_utils.h"

iGameItem::iGameItem()
{
	orientation = glm::quat(glm::vec3(0, 0, 0));
}

glm::quat iGameItem::getOrientationQ() { return orientation; }
glm::vec3 iGameItem::getOrientationEuler() { return glm::eulerAngles(orientation); }

void iGameItem::setOrientation(glm::quat newQOrientation) {
	orientation = newQOrientation;
}
void iGameItem::setOrientation(glm::vec3 newEulerOrientation) {
	orientation = glm::quat(newEulerOrientation);
}
void iGameItem::addOrientation(glm::quat deltaQOrientation) {
	//glm::quat newOrientation = orientation * deltaQOrientation;
	//printf("Recieved %s. New orientation: %s\n",
	//	glm::to_string(deltaQOrientation).c_str(),
	//	glm::to_string(newOrientation).c_str());
	this->orientation = orientation * deltaQOrientation;
}
void iGameItem::addOrientation(glm::vec3 deltaEulerOrientation) {
	addOrientation(glm::quat(glm::radians(deltaEulerOrientation)));
}

void iGameItem::setBaseDirection(glm::vec3 d) { baseDirection = glm::normalize(d); }

glm::vec3 iGameItem::getDirection() 
{
	return orientation * baseDirection;
}

glm::vec3 iGameItem::getBaseDirection() { return baseDirection; }

void iGameItem::setDirection(glm::vec3 dir)
{
	this->orientation = RotationBetweenVectors(this->baseDirection, dir);
	//printf("Setting direction of %s at: %s. quat: %s\nOriginally looking at %s\nNow looking at %s\n\n",
	//	getName().c_str(),
	//	glm::to_string(dir).c_str(),
	//	glm::to_string(this->orientation).c_str(),
	//	glm::to_string(this->baseDirection).c_str(),
	//	glm::to_string(getDirection()).c_str());
}