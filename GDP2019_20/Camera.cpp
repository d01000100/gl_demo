#include "GLCommon.h"
#include "globalStuff.h"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Camera.h"

Camera* Camera::theCamera = new Camera();

Camera::Camera() {
	pos = glm::vec3(1.0f) * 180.0f;
	target = glm::vec3(0.0f, 0.0, 0.0f);
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::init() {
	
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

void Camera::setTarget(glm::vec3 p_target) {
	target = p_target;
}

void Camera::setPosition(glm::vec3 position) {
	pos = position;
	
}

glm::vec3 Camera::getPosition() {
	return pos;
}
glm::vec3 Camera::getTarget() { return target; }


void Camera::moveUp(float angle) {
	setPosition(glm::rotate(direction(), -angle, right()) + target);
}

void Camera::moveDown(float angle) {
	setPosition(glm::rotate(direction(), angle, right()) + target);
}

void Camera::moveRight(float angle) {
	setPosition(glm::rotate(direction(), angle, upVector) + target);
}

void Camera::moveLeft(float angle) {
	setPosition(glm::rotate(direction(), -angle, upVector) + target);
}

void Camera::zoom(float distance) {
	glm::vec3 newDirection = direction() + distance * glm::normalize(direction());
	setPosition(target + newDirection);
}

void Camera::translate(glm::vec3 transDir) {
	float step = 1.0f;
	setPosition(pos + transDir * step);
	target += transDir * step;
}

glm::mat4 Camera::lookAt() {

	GLuint shaderProgID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

	// Also set the position of my "eye" into the shader
	GLint eyeLocation_UL = glGetUniformLocation(shaderProgID, "eyeLocation");
	glUniform4f(eyeLocation_UL, pos.x, pos.y, pos.z, 1.0f);

	return glm::lookAt(pos, target, upVector);
}
