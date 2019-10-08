#include "GLCommon.h"
#include "globalStuff.h"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Camera.h"

Camera* Camera::theCamera = new Camera();

Camera::Camera() {
	pos = glm::vec3(0.0, 80.0, -80.0);
	target = glm::vec3(0.0f, 0.0, 0.0f);
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 Camera::direction() {
	return pos - target;
}

glm::vec3 Camera::right() {
	return glm::normalize(glm::cross(upVector, direction()));
}

Camera* Camera::getTheCamera() {
	return theCamera;
}

void Camera::setTarget(glm::vec3 target) {
	this->target = target;
	if (glm::normalize(direction()) == upVector) {
		this->target += glm::vec3(1.0f, 1.0f, 1.0f);
	}
}

void Camera::setPosition(glm::vec3 pos) {
	this->pos = pos;
	if (glm::normalize(direction()) == upVector) {
		this->pos += glm::vec3(1.0f, 1.0f, 1.0f);
	}
}

glm::vec3 Camera::getPosition() {
	return pos;
}

void Camera::moveUp(float angle) {
	pos = glm::rotate(direction(), -angle, right());
	//upVector = glm::normalize(glm::cross(upVector, direction()));
}

void Camera::moveDown(float angle) {
	pos = glm::rotate(direction(), angle, right());
}

void Camera::moveRight(float angle) {
	pos = glm::rotate(direction(), angle, upVector);
}

void Camera::moveLeft(float angle) {
	pos = glm::rotate(direction(), -angle, upVector);
}

void Camera::zoom(float distance) {
	glm::vec3 newDirection = direction() + distance * glm::normalize(direction());
	pos = target + newDirection;
}

glm::mat4 Camera::lookAt() {
	return glm::lookAt(pos, target, upVector);

	GLuint shaderProgID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

	// Also set the position of my "eye" into the shader
	GLint eyeLocation_UL = glGetUniformLocation(shaderProgID, "eyeLocation");
	glUniform4f(eyeLocation_UL,
		theCamera->getPosition().x,
		theCamera->getPosition().y,
		theCamera->getPosition().z,
		1.0f);
}
