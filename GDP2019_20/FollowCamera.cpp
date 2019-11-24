#include "FollowCamera.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

FollowCamera* FollowCamera::theCamera = new FollowCamera();

FollowCamera::FollowCamera() {
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	this->targetItem = NULL;
	reposition();
}

FollowCamera* FollowCamera::getPhysicsCamera() {
	return FollowCamera::theCamera;
}

void FollowCamera::init(aGameItem* targetItem, glm::vec3 offset)
{
	this->targetItem = targetItem;
	setTarget(targetItem->getPos());
	this->offset = offset;
	reposition();
}

void FollowCamera::moveUp(float angle)
{
	offset = glm::rotate(direction(), -angle, right());
	reposition();
}

void FollowCamera::moveDown(float angle)
{
	offset = glm::rotate(direction(), angle, right());
	reposition();
}

void FollowCamera::moveRight(float angle)
{
	offset = glm::rotate(direction(), angle, upVector);
	reposition();
}

void FollowCamera::moveLeft(float angle)
{
	offset = glm::rotate(direction(), -angle, upVector);
	reposition();
}

void FollowCamera::reposition()
{
	if (targetItem != NULL) {
		//printf("itemTarget pos: %s\nOffset: %s\n", 
		//	glm::to_string(targetItem->getPos()).c_str(), 
		//	glm::to_string(offset).c_str());
		setTarget(targetItem->getPos());
		setPosition(targetItem->getPos() + offset);
	}
}