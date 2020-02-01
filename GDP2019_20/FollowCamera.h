#pragma once
#include <glm/vec3.hpp>
#include "Camera.h"
#include "cGameObject.h"

// A camera that follows a gameObject.
// It can also be rotated around that object.
// It stores a collection of gameObjects to iterate through and change targets.
class FollowCamera : public Camera
{
private:
	// Static Pointer for making a singleton
	static FollowCamera* theCamera;
	// The position of the camera relative to the target
	glm::vec3 mOffset;
	// Collection of possible targets
	std::vector<cGameObject*> mPhysicsObjects;
	// Current target
	std::vector<cGameObject*>::iterator mCurrentObject;
	// The constructor is private to enforce the use of Singleton
	FollowCamera();
	// Get the world position from the current target's "iPhysicsComponent"
	glm::vec3 getTargetPos();
public:
	// Static getter of the Singleton instance
	static FollowCamera* getPhysicsCamera();
	// Intialization of the Singleton instance.
	// Expects "theScene" Singleton to be already loaded.
	// offset : initial position of the camera relative to the target
	void init(glm::vec3 offset = glm::vec3(0, 50, -50));
	// Pitch the camera upwards around the target.
	// angle : how many radians to pitch.
	virtual void moveUp(float angle);
	// Pitch the camera downwards around the target.
	// angle : how many radians to pitch.
	virtual void moveDown(float angle);
	// Yaw the camera rightwards around the target.
	// angle : how many radians to yaw.
	virtual void moveRight(float angle);
	// Yaw the camera leftwards around the target.
	// angle : how many radians to yaw.
	virtual void moveLeft(float angle);
	// Changes the distance from the camera to the target.
	// scale : A positive number. The multiplier for the current distance.
	//         If it's between 0 and 1, it gets closer.
	//         If it's greater than 1, it gets farther.
	//         Negatives do unexpected results.
	virtual void zoom(float scale);
	// Updates the position of the camera according to the target's position.
	void update();
	// Changes target to the next object
	void next();
};

