#pragma once
#include "FollowCamera.h"

// Class to apply forces to the active iPhysicsComponent
// of the FollowCamera in relation of the direction the camera
// is looking at the object in the scene
class CameraPusher
{
private:
	// Reference to the singleton to simplify code
	FollowCamera *theCamera;
	// How hard to push on the objects each call
	float mPushForce;
	// Project a vector in the XZ plane. The "horizontal" plane
	glm::vec3 projectInFloor(vec3 v) const;
public:
	// Initializes the singleton reference
	CameraPusher();
	// Pushes the active object of the camera forward,
	// relative to the camera
	void pushForward();
	// Pushes the active object of the camera backwards,
	// relative to the camera
	void pushBackwards();
	// Pushes the active object of the camera right,
	// relative to the camera
	void pushRight();
	// Pushes the active object of the camera left,
	// relative to the camera
	void pushLeft();
};