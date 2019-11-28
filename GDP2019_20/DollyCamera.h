#pragma once

#include <glm/gtx/string_cast.hpp>
#include <sstream>
#include "GameItemFactory/iGameItem.h"

class DollyCamera : public aGameItem
{
private:
	static DollyCamera* theDollyCamera;
	DollyCamera();
protected:
	glm::vec3 pos;
public:
	static DollyCamera* getTheCamera();
	// aGameItem interface
	glm::vec3 getPos() { return pos; }
	void setPos(glm::vec3 p) { pos = p; }
	std::string getName() { return "theDollyCamera"; }
	void recieveMessage(sMessage message) {}
	std::string getType() { return "DollyCamera"; }
	virtual glm::mat4x4 calculateTransformationMatrix() {
		glm::mat4 res = glm::translate(glm::mat4(1.0f), getPos());

		res *= glm::mat4(getOrientationQ());

		return res;
	};
	virtual void draw() {};
	virtual std::string getInfo()
	{
		std::stringstream ss;
		ss << getType() << " - " << getName() <<
			" pos: " << glm::to_string(getPos()) <<
			" orientation: " << glm::to_string(getOrientationEuler()) << std::endl;
		return ss.str();
	}
	virtual json toJSON() { json res; return res; };
	glm::mat4 lookAt();
};