#include "GLCommon.h"
#include "globalStuff.h"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Camera.h"
#include "globalStuff.h"
#include "Colors.h"
#include "Scene.h"

Camera* Camera::theCamera = new Camera();

Camera::Camera() {
	pos = glm::vec3(0.0, 80.0, -80.0);
	target = glm::vec3(0.0f, 0.0, 0.0f);
	upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::setLights(std::string left, std::string right) {
	Scene* theScene = Scene::getTheScene();
	left_light = theScene->findLight(left);
	right_light = theScene->findLight(right);
}

void Camera::placeLights() {
	glm::vec3 lightOffSet = pos + glm::normalize(-direction()) * 2.0f;
	left_light->pos = lightOffSet + -right();
	right_light->pos = lightOffSet + right();

	left_light->direction = right_light->direction = -direction();
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
	placeLights();
}

void Camera::setPosition(glm::vec3 position) {
	pos = position;
	placeLights();
}

glm::vec3 Camera::getPosition() {
	return pos;
}

void Camera::moveUp(float angle) {
	pos = glm::rotate(direction(), -angle, right()) + target;
}

void Camera::moveDown(float angle) {
	pos = glm::rotate(direction(), angle, right()) + target;
}

void Camera::moveRight(float angle) {
	pos = glm::rotate(direction(), angle, upVector) + target;
}

void Camera::moveLeft(float angle) {
	pos = glm::rotate(direction(), -angle, upVector) + target;
}

void Camera::zoom(float distance) {
	glm::vec3 newDirection = direction() + distance * glm::normalize(direction());
	pos = target + newDirection;
}

glm::mat4 Camera::lookAt() {

	GLuint shaderProgID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

	// Also set the position of my "eye" into the shader
	GLint eyeLocation_UL = glGetUniformLocation(shaderProgID, "eyeLocation");
	glUniform4f(eyeLocation_UL, pos.x, pos.y, pos.z, 1.0f);

	return glm::lookAt(pos, target, upVector);
}

// navigation camera
void Camera::turnVertically(float angle) {
	target = glm::rotate(-direction(), angle, right()) + pos;
	placeLights();
}
void Camera::turnHorizontally(float angle) {
	target = glm::rotate(-direction(), angle, upVector) + pos;
	placeLights();
}
void Camera::walk(float distance) {
	glm::vec3 forward = glm::normalize(-direction());
	target += forward * distance;
	pos += forward * distance;
	placeLights();
}

void Camera::debugTarget() {
	::theDebugRenderer->addLine(target, target + glm::vec3(1.0f, 0.0f, 0.0f), Colors::yellow, 0.1f);
	::theDebugRenderer->addLine(target, target + glm::vec3(0.0f, 1.0f, 0.0f), Colors::blue, 0.1f);
	::theDebugRenderer->addLine(target, target + glm::vec3(0.0f, 0.0f, 1.0f), Colors::red, 0.1f);
}
	