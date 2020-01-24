#pragma once
#include "cGameObject.h"
#include "Scene.h"

class Gameplay
{
public:
	Scene *theScene;
	cGameObject* playerBullet, *player;
	GLFWwindow* window;
	float shootTimer;
	void init(GLFWwindow* w);
	void Shoot();
	void velocityControls();
	void update(float deltaTime);
};

