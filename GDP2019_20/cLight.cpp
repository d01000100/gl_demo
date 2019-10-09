#include "cLight.h"

cLight::cLight() {
	type = SPOT;
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	constAtten = 0.0000001f;
	linearAtten = 1;
	quadAtten = 1;
	diffuseColor = glm::vec3(0.0f, 0.0f, 0.0f);
	// r, g, b, shininess [1.0f, 10000f)
	specularColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	cutOffDist = 1000000.0f;
	isOn = true;

	// for spotlights
	direction = glm::vec3(0.0f, 0.0f, 1.0f);;
	innerAngle = 0.0f;
	outerAngle = 0.0f;
}