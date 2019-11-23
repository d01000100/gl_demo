#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include "../GLCommon.h"
#include "../Messages/iMessagable.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class iGameItem : public iMessagable {
private:
	glm::quat orientation;
	glm::vec3 baseDirection;
protected:
	iGameItem();
public:
	virtual glm::vec3 getPos() = 0;
	virtual std::string getName() = 0;
	virtual void recieveMessage(sMessage message) = 0;
	virtual std::string getType() = 0;
	virtual glm::mat4x4 calculateTransformationMatrix() = 0;
	virtual void draw() = 0;
	virtual std::string getInfo() = 0;
	virtual json toJSON() = 0;

	glm::quat getOrientationQ();
	glm::vec3 getOrientationEuler();
	void setOrientation(glm::quat newQOrientation);
	void setOrientation(glm::vec3 newEulerOrientation);
	void addOrientation(glm::quat deltaQOrientation);
	void addOrientation(glm::vec3 deltaEulerOrientation);
	void setBaseDirection(glm::vec3 dir);
	glm::vec3 getDirection();
};