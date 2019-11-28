#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include "../GLCommon.h"
#include "../Messages/iMessagable.h"
#include "../aOrientable.h"
#include <nlohmann/json.hpp>
#include "../colors.h"

using json = nlohmann::json;

class aGameItem : public aOrientable, public iMessagable {
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

	void debugOrientation()
	{
		debugOrientationAxis(getPos());
	}
};