#pragma once
#include "Coordinator.h"
#include "PathFollowing.h"
#include "Flocking.h"

struct FlockingManager
{
	Coordinator* formationCoordinator = nullptr;
	PathFollowing pathManager;
	cGameObject* leader = nullptr;
	std::string state = "Formation";
	bool is9Pressed = false;
	Flocking flock;
	
	~FlockingManager();

	void init();

	void update(float deltaTime);

	void userInput();

	void thorusGeometry(cGameObject* vehicle);
};

