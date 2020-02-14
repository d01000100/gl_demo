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
	leader = (cGameObject*)theScene->findItem("player");
	formationCoordinator = new Coordinator(leader);
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
	if (state == "Path")
	{
		formationCoordinator->update(deltaTime);
		pathManager.followPath(leader, deltaTime);
	}
	thorusGeometry(leader);
}

void FlockingManager::userInput()
{
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		formationCoordinator->changeFormation(Coordinator::circle);
		state = "Formation";
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		formationCoordinator->changeFormation(Coordinator::vFormation);
		state = "Formation";
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		formationCoordinator->changeFormation(Coordinator::square);
		state = "Formation";
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		formationCoordinator->changeFormation(Coordinator::line);
		state = "Formation";
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		formationCoordinator->changeFormation(Coordinator::twoLines);
		state = "Formation";
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	{
		state = "Path";
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
	{
		if (!is9Pressed)
		{
			state = "Path";
			pathManager.inversePath();
		}
		is9Pressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_RELEASE)
	{
		is9Pressed = false;
	}
	if (state == "Formation")
	{
		velocityControls("player", ::window);
	}
}

void FlockingManager::thorusGeometry(cGameObject* vehicle)
{
	// Size of the stage is 84.7 units across in both sides
	float stageLength = 84.7f;
	float minX = -stageLength / 2,
		maxX = stageLength / 2,
		minz = -stageLength / 2,
		maxz = stageLength / 2;

	glm::vec3 pos = vehicle->getPos();

	if (pos.x < minX)
		pos.x = maxX;
	if (pos.x > maxX)
		pos.x = minX;
	if (pos.z < minz)
		pos.z = maxz;
	if (pos.z > maxz)
		pos.z = minz;

	vehicle->position = pos;
}
