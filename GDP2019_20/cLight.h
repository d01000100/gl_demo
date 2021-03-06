#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

#include "GameItemFactory/iGameItem.h"

enum lightType {
	POINT = 0,
	SPOT = 1,
	DIRECTIONAL = 2,
};

class cLight : public aGameItem
{
public:
	lightType type;
	glm::vec3 pos;
	float constAtten;
	float linearAtten;
	float quadAtten;
	glm::vec3 diffuseColor;
	glm::vec4 specularColor;
	float cutOffDist;
	bool isOn;
	std::string name;

	// for spotlights
	float innerAngle;
	float outerAngle;

	cLight();
	int getUniqueId();

	// from iGameItem
	virtual glm::mat4x4 calculateTransformationMatrix() ;
	virtual void draw() ;
	virtual std::string getName() ;
	virtual glm::vec3 getPos() ;
	void setPos(glm::vec3);
	virtual void recieveMessage(sMessage message) ;
	virtual std::string getType() ;

	std::string getInfo();
	json toJSON();

	static void resetIds();

private:
	static int next_unique_id;
	int unique_id;
};

