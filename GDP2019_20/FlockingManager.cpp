#include "FlockingManager.h"
#include "globalStuff.h"
#include "Scene.h"
#include "UserInput.h"

FlockingManager::~FlockingManager()
{
	delete formationCoordinator;
}

void FlockingManager::init()
{
	Scene* theScene = Scene::getTheScene();
	formationCoordinator = new Coordinator((cGameObject*)theScene->findItem("player"));
	cGameObject* vehicleTemplate = (cGameObject*)theScene->findItem("ship_template");
	for (int i = 0; i < 11; i++)
	{
		cGameObject* vehicle = new cGameObject(vehicleTemplate);
		vehicle->isVisible = true;
		formationCoordinator->batallion.push_back(vehicle);
		theScene->addItem(vehicle);
	}
}

void FlockingManager::update(float deltaTime)
{
	userInput();
	if (state == "Formation")
		formationCoordinator->update(deltaTime);
}

void FlockingManager::userInput()
{
	if (state == "Formation")
	{
		velocityControls("player", ::window);
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			formationCoordinator->changeFormation(Coordinator::circle);
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			formationCoordinator->changeFormation(Coordinator::vFormation);
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			formationCoordinator->changeFormation(Coordinator::square);
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
			formationCoordinator->changeFormation(Coordinator::line);
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
			formationCoordinator->changeFormation(Coordinator::twoLines);
	}
}
