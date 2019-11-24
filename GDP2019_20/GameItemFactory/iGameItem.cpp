#include "iGameItem.h"

#include <glm/gtx/string_cast.hpp>

#include "../quaternions_utils.h"

aGameItem::aGameItem()
{
	orientation = glm::quat(glm::vec3(0, 0, 0));
	baseDirection = glm::vec3(0, 0, 1);
	baseUp = glm::vec3(0, 1, 0);
}

glm::quat aGameItem::getOrientationQ() { return orientation; }
glm::vec3 aGameItem::getOrientationEuler() { return glm::eulerAngles(orientation); }

void aGameItem::setOrientation(glm::quat newQOrientation) {
	orientation = newQOrientation;
}
void aGameItem::setOrientation(glm::vec3 newEulerOrientation) {
	orientation = glm::quat(newEulerOrientation);
}
void aGameItem::addOrientation(glm::quat deltaQOrientation) {
	//glm::quat newOrientation = orientation * deltaQOrientation;
	//printf("Recieved %s. New orientation: %s\n",
	//	glm::to_string(deltaQOrientation).c_str(),
	//	glm::to_string(newOrientation).c_str());
	this->orientation = orientation * deltaQOrientation;
}
void aGameItem::addOrientation(glm::vec3 deltaEulerOrientation) {
	addOrientation(glm::quat(glm::radians(deltaEulerOrientation)));
}

glm::vec3 aGameItem::getDirection() 
{
	return orientation * baseDirection;
}

void aGameItem::setDirection(glm::vec3 dir)
{
	this->orientation = RotationBetweenVectors(this->baseDirection, dir);
	//printf("Setting direction of %s at: %s. quat: %s\nOriginally looking at %s\nNow looking at %s\n\n",
	//	getName().c_str(),
	//	glm::to_string(dir).c_str(),
	//	glm::to_string(this->orientation).c_str(),
	//	glm::to_string(this->baseDirection).c_str(),
	//	glm::to_string(getDirection()).c_str());
}