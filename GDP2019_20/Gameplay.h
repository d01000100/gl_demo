#pragma once
#include "cGameObject.h"
#include "Scene.h"
#include "NPC_AI.h"

class Gameplay
{
public:
	Scene *theScene;
	cGameObject* playerBullet, *player;
	GLFWwindow* window;
	NPC_AI* npc_AI;
	float shootTimer, fireCooldown;
	void init(GLFWwindow* w);
	void Shoot();
	void Collisions();
	void velocityControls();
	void update(float deltaTime);
};

