#pragma once
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include "cLight.h"

class Camera
{
private:
	glm::vec3 pos;
	glm::vec3 target;
	glm::vec3 upVector;
	static Camera* theCamera;
	Camera();
	glm::vec3 direction();
	glm::vec3 right();
	void placeLights();
	cLight *left_light, *right_light;
public:
	static Camera* getTheCamera();
	void setTarget(glm::vec3 target);
	void setPosition(glm::vec3 pos);
	glm::vec3 getPosition();
	void moveUp(float angle);
	void moveDown(float angle);
	void moveRight(float angle);
	void moveLeft(float angle);
	void zoom(float distance);
	glm::mat4 lookAt();
	void setLights(std::string left, std::string right);

	// navigation camera
	void turnVertically(float angle);
	void turnHorizontally(float angle);
	void walk(float distance);

	void debugTarget();
};

