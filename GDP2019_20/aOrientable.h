#pragma once
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include "quaternions_utils.h"
#include "colors.h"

class aOrientable 
{
protected:
	glm::quat orientation;
	glm::vec3 baseDirection, baseUp;
public:
	aOrientable() {
		orientation = glm::quat(glm::vec3(0, 0, 0));
		baseDirection = glm::vec3(0, 0, 1);
		baseUp = glm::vec3(0, 1, 0);
	}
	glm::quat getOrientationQ() { return orientation; };
	glm::vec3 getOrientationEuler() {
		return glm::degrees(glm::eulerAngles(orientation));
	}
	void setOrientation(glm::quat newQOrientation) {
		orientation = newQOrientation;
	}
	void setOrientation(glm::vec3 newEulerOrientation) {
		orientation = glm::quat(glm::radians(newEulerOrientation));
	}
	void addOrientation(glm::quat deltaQOrientation) {
		orientation = orientation * deltaQOrientation;
	}
	void addOrientation(glm::vec3 deltaEulerOrientation) {
		addOrientation(glm::quat(glm::radians(deltaEulerOrientation)));
	}

	glm::vec3 getDirection() {
		return orientation * baseDirection;
	}
	glm::vec3 getBaseDirection() { return baseDirection; }
	glm::vec3 getUp() { return orientation * baseUp; }
	glm::vec3 getRight() { return cross(getDirection(), getUp()); }
	void setDirection(glm::vec3 dir)
	{
		if (glm::length(dir) > 0)
			this->orientation = RotationBetweenVectors(
				this->baseDirection, 
				glm::normalize(dir));
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