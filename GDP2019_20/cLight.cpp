#include "cLight.h"

cLight::cLight() {
	type = SPOT;
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	const_atten = 0;
	linear_atten = 1;
	quad_atten = 1;
	diffuseColor = glm::vec3(0.0f, 0.0f, 0.0f);
	// r, g, b, shininess [1.0f, 10000f)
	specularColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	cutOffDist = 1000000.0f;
	isOn = true;

	// for spotlights
	direction = glm::vec3(0.0f, 0.0f, 1.0f);;
	inner_angle = 0.0f;
	outer_angle = 0.0f;
}