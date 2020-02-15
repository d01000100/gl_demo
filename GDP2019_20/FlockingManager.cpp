#include "FlockingManager.h"
#include "globalStuff.h"
#include "Scene.h"
#include "UserInput.h"
#include <sstream>

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
		vehicle->physics->shape = SPHERE;
		formationCoordinator->batallion.push_back(vehicle);
		theScene->addItem(vehicle);
	}
	flock.birbs = formationCoordinator->batallion;
	flock.birbs.push_back(leader);
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
	if (state == "Flock")
	{
		flock.update(deltaTime);
		for (auto birb : flock.birbs)
			thorusGeometry(birb);
	}

	std::stringstream info;
	info << "Alignment: " << flock.alignmentPower <<
		" Separation: " << flock.separationPower <<
		" Cohesion: " << flock.cohesionPower;
	glfwSetWindowTitle(::window, info.str().c_str());
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
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		state = "Flock";
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
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

	// Flocking controls
	if (state == "Flock")
	{
		float min = 0, max = 5, step = 0.01;
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		{
			if (flock.separationPower < max)
			{
				flock.separationPower += step;
				flock.separationPower = glm::min(flock.separationPower, max);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		{
			if (flock.separationPower > min)
			{
				flock.separationPower -= step;
				flock.separationPower = glm::max(flock.separationPower, min);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		{
			if (flock.cohesionPower < max)
			{
				flock.cohesionPower += step;
				flock.cohesionPower = glm::min(flock.cohesionPower, max);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		{
			if (flock.cohesionPower > min)
			{
				flock.cohesionPower -= step;
				flock.cohesionPower = glm::max(flock.cohesionPower, min);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			if (flock.alignmentPower < max)
			{
				flock.alignmentPower += step;
				flock.alignmentPower = glm::min(flock.alignmentPower, max);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			if (flock.alignmentPower > min)
			{
				flock.alignmentPower -= step;
				flock.alignmentPower = glm::max(flock.alignmentPower, min);
			}
		}
	}
	
	if (state == "Formation")
	{
		velocityControls("player", ::window);
	}
}

void FlockingManager::thorusGeometry(cGameObject* vehicle)
{
	// Size of the stage is 84.7 units across in both sides
	float stageLength = 70.f;
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
