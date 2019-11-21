#pragma once
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4
#include "audio_listener.h"

class Camera
{
private:
	static Camera* theCamera;
	glm::vec3 pos;
protected:
	Camera();
	glm::vec3 target;
	glm::vec3 upVector;
	AudioListener* audioListener;
	glm::vec3 direction();
	glm::vec3 right();
	void updateListener();
public:
	static Camera* getTheCamera();
	void init();
	void setTarget(glm::vec3 target);
	void setPosition(glm::vec3 pos);
	glm::vec3 getPosition();
	glm::vec3 getTarget();
	virtual void moveUp(float angle);
	virtual void moveDown(float angle);
	virtual void moveRight(float angle);
	virtual void moveLeft(float angle);
	void translate(glm::vec3);
	virtual void zoom(float distance);
	glm::mat4 lookAt();
};

