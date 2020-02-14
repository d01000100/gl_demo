#pragma once
#include "Coordinator.h"

struct FlockingManager
{
	Coordinator* formationCoordinator = nullptr;
	std::string state = "Formation";
	
	~FlockingManager();

	void init();

	void update(float deltaTime);

	void userInput();
};

