#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include "../GLCommon.h"
#include "../Messages/iMessagable.h"
#include <nlohmann/json.hpp>

#include "../globalStuff.h"
#include "../colors.h"

using json = nlohmann::json;

class aGameItem : public iMessagable {
private:
	glm::quat orientation;
	glm::vec3 baseDirection, baseUp;
protected:
	aGameItem();
public:
	virtual glm::vec3 getPos() = 0;
	virtual void setPos(glm::vec3) = 0;
	virtual std::string getName() = 0;
	virtual void recieveMessage(sMessage message) = 0;
	virtual std::string getType() = 0;
	virtual glm::mat4x4 calculateTransformationMatrix() = 0;
	virtual void draw() = 0;
	virtual std::string getInfo() = 0;
	virtual json toJSON() = 0;

	// TODO: Move to aOrientable
	glm::quat getOrientationQ();
	glm::vec3 getOrientationEuler();
	void setOrientation(glm::quat newQOrientation);
	void setOrientation(glm::vec3 newEulerOrientation);
	void addOrientation(glm::quat deltaQOrientation);
	void addOrientation(glm::vec3 deltaEulerOrientation);

	glm::vec3 getDirection();
	glm::vec3 getBaseDirection() { return baseDirection; }
	glm::vec3 getUp() { return orientation * baseUp; }
	glm::vec3 getRight() { return cross(getDirection(), getUp()); }
	void setDirection(glm::vec3 dir);

	void debugOrientationAxis()
	{
		glm::vec3 origin = getPos();
		::g_pDebugRenderer->addLine(origin, origin + getDirection() * 7.0f, Colors::red, 0.01f);
		::g_pDebugRenderer->addLine(origin, origin + getUp() * 5.0f, Colors::yellow, 0.01f);
		::g_pDebugRenderer->addLine(origin, origin + getRight() * 3.0f, Colors::blue, 0.01f);
	}

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