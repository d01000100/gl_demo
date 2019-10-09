#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

enum lightType {
	POINT = 0,
	SPOT = 1,
	DIRECTIONAL = 2,
};

class cLight
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

	// for spotlights
	glm::vec3 direction;
	float innerAngle;
	float outerAngle;

	cLight();
};

