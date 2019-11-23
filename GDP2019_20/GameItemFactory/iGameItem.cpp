#include "iGameItem.h"

iGameItem::iGameItem()
{
	//orientation = glm::quat(0, 0, 0, 1);
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
	return baseDirection * orientation;
}