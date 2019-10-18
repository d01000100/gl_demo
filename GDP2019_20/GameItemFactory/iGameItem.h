#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include "../GLCommon.h"
#include "../Messages/iMessagable.h"

class iGameItem : public iMessagable {
public:
	virtual glm::mat4x4 calculateTransformationMatrix() = 0;
	virtual void draw() = 0;
	virtual std::string getName() = 0;
	virtual glm::vec3 getPos() = 0;
	virtual void recieveMessage(sMessage message) = 0;
	virtual std::string getType() = 0;
};