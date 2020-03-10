#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include "quaternions_utils.h"
#include "colors.h"
#include <mutex>

class aOrientable 
{
protected:
	glm::quat orientation;
	glm::vec3 baseDirection, baseUp;
	std::mutex orientationMutex;
public:
	aOrientable() {
		orientation = glm::quat(glm::vec3(0, 0, 0));
		baseDirection = glm::vec3(0, 0, 1);
		baseUp = glm::vec3(0, 1, 0);
	}
	glm::quat getOrientationQ()
	{
		orientationMutex.lock();
		auto res = orientation;
		orientationMutex.unlock();
		return res;
	};
	glm::vec3 getOrientationEuler() {
		return glm::degrees(glm::eulerAngles(getOrientationQ()));
	}
	void setOrientation(glm::quat newQOrientation) {
		orientationMutex.lock();
		orientation = newQOrientation;
		orientationMutex.unlock();
	}
	void setOrientation(glm::vec3 newEulerOrientation) {
		setOrientation(glm::quat(glm::radians(newEulerOrientation)));
	}
	void addOrientation(glm::quat deltaQOrientation) {
		orientationMutex.lock();
		orientation = orientation * deltaQOrientation;
		orientationMutex.unlock();
	}
	void addOrientation(glm::vec3 deltaEulerOrientation) {
		addOrientation(glm::quat(glm::radians(deltaEulerOrientation)));
	}

	glm::vec3 getDirection() {
		return getOrientationQ() * baseDirection;
	}
	glm::vec3 getBaseDirection() { return baseDirection; }
	glm::vec3 getUp() { return getOrientationQ() * baseUp; }
	glm::vec3 getRight() { return cross(getDirection(), getUp()); }
	void setDirection(glm::vec3 dir)
	{
		if (glm::length(dir) > 0)
			setOrientation(
				RotationBetweenVectors(
					this->baseDirection, 
					glm::normalize(dir)));
	}

	virtual void debugOrientationAxis(glm::vec3 origin);

	void pitch(float deltaAngle) {
		addOrientation(glm::vec3(deltaAngle, 0, 0));
	}

	void yaw(float deltaAngle) {
		addOrientation(glm::vec3(0, deltaAngle, 0));
	}

	void barrelRoll(float deltaAngle) {
		addOrientation(glm::vec3(0, 0, deltaAngle));
	}
};
