#include "cLight.h"
#include "globalStuff.h"

#include <glm/mat4x4.hpp>
#include <sstream>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

int cLight::next_unique_id = 0;

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
	direction = glm::vec3(0.0f, 0.0f, 1.0f);
	innerAngle = 0.0f;
	outerAngle = 0.0f;

	unique_id = next_unique_id;
	next_unique_id++;
}

glm::mat4x4 cLight::calculateTransformationMatrix() {
	return glm::mat4x4(1.0f);
}

void cLight::draw() {
	GLuint shaderProgID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

	// Set the lighting values for the shader.
	// See fragmentShader for documentation
	std::string prefix = "theLights[" + std::to_string(unique_id) + "].";
	GLint L_0_position = glGetUniformLocation(shaderProgID, (prefix + "position").c_str());
	GLint L_0_diffuse = glGetUniformLocation(shaderProgID, (prefix + "diffuse").c_str());
	GLint L_0_specular = glGetUniformLocation(shaderProgID, (prefix + "specular").c_str());
	GLint L_0_atten = glGetUniformLocation(shaderProgID, (prefix + "atten").c_str());
	GLint L_0_direction = glGetUniformLocation(shaderProgID, (prefix + "direction").c_str());
	GLint L_0_param1 = glGetUniformLocation(shaderProgID, (prefix + "param1").c_str());
	GLint L_0_param2 = glGetUniformLocation(shaderProgID, (prefix + "param2").c_str());

	glUniform4f(L_0_position,
		pos.x,
		pos.y,
		pos.z,
		1.0f);
	glUniform4f(L_0_diffuse, diffuseColor.r, diffuseColor.g, diffuseColor.b, 1.0f);
	glUniform4f(L_0_specular, specularColor.r, specularColor.g, specularColor.b, specularColor.a);
	glUniform4f(L_0_atten, constAtten,
		linearAtten,
		quadAtten,
		cutOffDist);

	// x = lightType, y = inner angle, z = outer angle, w = TBD
	// 0 = pointlight
	// 1 = spot light
	// 2 = directional light
	glUniform4f(L_0_param1, type , innerAngle, outerAngle, 1.0f);
	glUniform4f(L_0_param2, isOn , 0.0f, 0.0f, 1.0f);

	glUniform4f(L_0_direction,
		direction.x,
		direction.y,
		direction.z,
		1.0f);
}

std::string cLight::getName() { return name; }
glm::vec3 cLight::getPos() { return pos; }
void cLight::recieveMessage(sMessage message){ 
	float translationStep = 1.0f;
	float rotationStep = 0.02;
	float scaleStep = 0.001f;

	printf("Light recieving message %s with %f\n", message.name.c_str(), message.fValue);

	if (message.name == "translate") {
		pos += glm::normalize(message.v3Value) * translationStep;
	}
	else if (message.name == "rotate") {
		this->direction += glm::normalize(message.v3Value) * rotationStep;
		direction = glm::normalize(direction);
	}
	else if (message.name == "scale") {
		float linearStep = 0.1f;
		if (message.fValue > 0) {
			linearAtten *= 1.0f + linearStep;
		}
		if (message.fValue < 0) {
			linearAtten *= 1.0f - linearStep;
		}

		linearAtten = std::min(linearAtten, 1.0f);
		linearAtten = std::max(linearAtten, 0.0000000000000000000000000001f);
	}
	if (message.name == "quadAtten") {
		float quadStep = 0.1f;
		if (message.fValue > 0) {
			quadAtten *= 1.0f + quadStep;
		}
		if (message.fValue < 0) {
			quadAtten *= 1.0f - quadStep;
		}

		quadAtten = std::min(quadAtten, 1.0f);
		quadAtten = std::max(quadAtten, 0.0000000000000000000000000001f);
	}
	if (message.name == "innerAngle") {
		innerAngle += message.fValue * 5.0f;
	}
	if (message.name == "outerAngle") {
		outerAngle += message.fValue * 5.0f;
	}
}
std::string cLight::getType() { return "Light"; }
int cLight::getUniqueId() { return unique_id; }

std::string cLight::getInfo() {
	std::stringstream ss;
	ss << getType() << " - " << getName() <<
		" pos: " << pos.x << ", " << pos.z << ", " << pos.z << ", " <<
		" dir: " << direction.x << ", " << direction.y << ", " << direction.z << ", " << 
		" color: " << glm::to_string(diffuseColor) <<
		" linearAttern: " << linearAtten << " quadAtten: " << quadAtten;
	if (type == SPOT) {
		ss << " innerAngle: " << innerAngle <<
			" outerAngle: " << outerAngle;
	}
	return ss.str();
};

json cLight::toJSON() {
	json jLight;

	jLight["name"] = name;
		
	jLight["position"][0] = pos.x; 
	jLight["position"][1] = pos.y; 
	jLight["position"][2] = pos.z; 

	jLight["diffuseColor"][0] = diffuseColor.x * 255.f;
	jLight["diffuseColor"][1] = diffuseColor.y * 255.f;
	jLight["diffuseColor"][2] = diffuseColor.z * 255.f;

	jLight["specularColor"][0] = specularColor.x * 255.f;
	jLight["specularColor"][1] = specularColor.y * 255.f;
	jLight["specularColor"][2] = specularColor.z * 255.f;
	jLight["specularColor"][3] = specularColor.w;

	jLight["linearAtten"] = linearAtten;

	jLight["quadAtten"] = quadAtten;

	jLight["cutOffDist"] = cutOffDist;

	switch (type) {
		case POINT:
			jLight["type"] = "point";
			break;
		case SPOT:
			jLight["type"] = "spotlight";
			break;
		case DIRECTIONAL:
			jLight["type"] = "directional";
			break;
	}

	jLight["isOn"] = isOn;

	jLight["direction"][0] = direction.x;
	jLight["direction"][1] = direction.y;
	jLight["direction"][2] = direction.z;

	jLight["innerAngle"] = innerAngle;

	jLight["outerAngle"] = outerAngle;

	return jLight;
}