#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include "FollowCamera.h"
#include "Scene.h"
#include <iostream>

FollowCamera* FollowCamera::theCamera = new FollowCamera();

FollowCamera::FollowCamera() {
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 FollowCamera::getTargetPos()
{
	if (!mPhysicsObjects.empty())
	{
		glm::mat4 targetTransform;
		(*mCurrentObject)->mPhysicsCompoment->GetTransform(targetTransform);
		return glm::vec3(targetTransform[3].x, targetTransform[3].y, targetTransform[3].z);
	}
	else
	{
		return glm::vec3(0);
	}
}

FollowCamera* FollowCamera::getPhysicsCamera() {
	return FollowCamera::theCamera;
}

void FollowCamera::init(glm::vec3 offset)
{
	this->mOffset = offset;
	Scene* theScene = Scene::getTheScene();
	if (theScene == nullptr)
	{
		std::cout << "Follow Camera initialized without Scene ready!!\n";
		mCurrentObject = mPhysicsObjects.end();
		return;
	}
	mPhysicsObjects.clear();
	auto gameItems = theScene->getItems();
	for (auto i = gameItems.begin();
		i != gameItems.end(); i++)
	{
		aGameItem* item = *i;
		if (item->getType() == "Object")
		{
			cGameObject* go = dynamic_cast<cGameObject*>(item);
			if (go->mPhysicsCompoment && 
				go->mPhysicsCompoment->GetComponentType() == nPhysics::eComponentType::ball)
			{
				mPhysicsObjects.push_back(go);
			}
		}
	}
	if (mPhysicsObjects.empty())
	{
		std::cout << "Follow Camera has no objects to follow\n";
		return;
	}
	mCurrentObject = mPhysicsObjects.begin();
	setTarget(getTargetPos());
	update();
}

void FollowCamera::moveUp(float angle)
{
	mOffset = glm::rotate(direction(), -angle, right());
	update();
}

void FollowCamera::moveDown(float angle)
{
	mOffset = glm::rotate(direction(), angle, right());
	update();
}

void FollowCamera::moveRight(float angle)
{
	mOffset = glm::rotate(direction(), angle, upVector);
	update();
}

void FollowCamera::moveLeft(float angle)
{
	mOffset = glm::rotate(direction(), -angle, upVector);
	update();
}

void FollowCamera::zoom(float scale)
{
	if (scale > 0)
	{
		if (scale < 1 && glm::length(mOffset) > 10)
			// if we're zooming in and we're not super close
			mOffset *= scale;
		else if (scale > 1 && glm::length(mOffset) < 500)
			// if we're zooming out and we're not super far
			mOffset *= scale;
	}
}

void FollowCamera::update()
{
	//printf("itemTarget pos: %s\nOffset: %s\n", 
	//	glm::to_string(mTargetComponent->getPos()).c_str(), 
	//	glm::to_string(mOffset).c_str());
	glm::vec3 targetPos = getTargetPos();
	setTarget(targetPos);
	setPosition(targetPos + mOffset);
}

void FollowCamera::next()
{
	if (mPhysicsObjects.size() > 1)
	{
		// If we have at least two objects
		mCurrentObject++;
		if (mCurrentObject == mPhysicsObjects.end())
			mCurrentObject = mPhysicsObjects.begin();
	}
}
