#pragma once
#include "Coordinator.h"
#include "PathFollowing.h"

struct FlockingManager
{
	Coordinator* formationCoordinator = nullptr;
	PathFollowing pathManager;
	cGameObject* leader = nullptr;
	std::string state = "Path";
	bool is9Pressed = false;
	
	~FlockingManager();

	void init();

	void update(float deltaTime);

	void userInput();

	void thorusGeometry(cGameObject* vehicle);
};

