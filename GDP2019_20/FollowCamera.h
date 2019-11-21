#pragma once
#include <glm/vec3.hpp>
#include "Camera.h"
#include "GameItemFactory/iGameItem.h"
/*
	A camera that follows a gameObject.
	It can also be rotated around that object.
*/
class FollowCamera : public Camera
{
private:
	static FollowCamera* theCamera;
	glm::vec3 offset;
	iGameItem* targetItem;
	FollowCamera();
public:
	static FollowCamera* getPhysicsCamera();
	void init(iGameItem* target, glm::vec3 offset = glm::vec3(0, 50, -50));
	virtual void moveUp(float angle);
	virtual void moveDown(float angle);
	virtual void moveRight(float angle);
	virtual void moveLeft(float angle);
	/*
		Repositions the camera in the offset of the target.
	*/
	void reposition();
};

