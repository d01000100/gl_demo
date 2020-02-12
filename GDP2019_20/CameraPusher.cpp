#include "CameraPusher.h"

glm::vec3 CameraPusher::projectInFloor(glm::vec3 v) const
{
	v.y = 0;
	return v;
}

CameraPusher::CameraPusher()
{
	theCamera = FollowCamera::getFollowCamera();
	mPushForce = 10.f;
}

void CameraPusher::pushForward()
{
	(*theCamera->mCurrentObject)->mPhysicsCompoment->ApplyForce(glm::normalize(projectInFloor(-theCamera->direction())) * mPushForce);
}

void CameraPusher::pushBackwards()
{
	(*theCamera->mCurrentObject)->mPhysicsCompoment->ApplyForce(glm::normalize(projectInFloor(theCamera->direction())) * mPushForce);
}

void CameraPusher::pushRight()
{
	(*theCamera->mCurrentObject)->mPhysicsCompoment->ApplyForce(glm::normalize(projectInFloor(theCamera->right())) * mPushForce);
}

void CameraPusher::pushLeft()
{
	(*theCamera->mCurrentObject)->mPhysicsCompoment->ApplyForce(glm::normalize(projectInFloor(-theCamera->right())) * mPushForce);
}
