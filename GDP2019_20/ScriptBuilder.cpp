#include "ScriptBuilder.h"
#include "MoveCommand.h"
#include "SerialCommand.h"
#include "ParallelCommand.h"
#include "RotateCommand.h"
#include "LookToward.h"
#include "FollowCommand.h"
#include "RideCurve.h"
#include "TriggerCommand.h"
#include "Scene.h"
#include "DollyCamera.h"

iCommand* TestScript()
{
	Scene* theScene = Scene::getTheScene();
	aGameItem* ship1 = theScene->findItem("ship1");
	aGameItem* ship2 = theScene->findItem("ship2");
	aGameItem* player = theScene->findItem("player");
	aGameItem* camera = DollyCamera::getTheCamera();
	SerialCommand* script = new SerialCommand();
	ParallelCommand* allUp = new ParallelCommand();
	camera->setPos(glm::vec3(0, 100, -150));
	camera->setDirection(-glm::vec3(0, 100, -150));
	script->pushCommand(new MoveCommand(camera, glm::vec3(0, 100, -150), glm::vec3(0, 90, -140), 3.0f, 1, 1));
	//script->pushCommand(new RotateCommand(camera, glm::vec3(0,0,10), 1.0f));
	//script->pushCommand(new MoveCommand(ship, glm::vec3(0, 34, -20), glm::vec3(20, 30, 20), 5.0f, 1.0f, 1.5f));
	//script->pushCommand(new MoveCommand(player, glm::vec3(10, 10, -10), glm::vec3(0, 20, 0), 2.0f, 0.0f, 1.0f));
	allUp->addCommand(new RideCurve(ship1, glm::vec3(-50, 30, 0), glm::vec3(50, 30, 0), glm::vec3(0, 120, 0), 12, 4, 4));
	allUp->addCommand(new RideCurve(ship2, glm::vec3(-50, 30, 30), glm::vec3(50, 30, 30), glm::vec3(0, 120, 0), 12, 0, 0));
	SerialCommand* cameraPan = new SerialCommand();
	cameraPan->pushCommand(new RotateCommand(camera, glm::vec3(-20, 0, 0), 6, 4, 2));
	cameraPan->pushCommand(new RotateCommand(camera, glm::vec3(20, 0, 0), 6, 2, 4));
	allUp->addCommand(cameraPan);
	//allUp->addCommand(new FollowCommand(player, ship, glm::vec3(0,10,0), 20, 30, 60, 0));
	//allUp->addCommand(new TriggerCommand(ship1, glm::vec3(-10), glm::vec3(10), new RotateCommand(ship2, glm::vec3(0, 170, 0), 7.0), 2));
	//allUp->addCommand(new MoveCommand(player, glm::vec3(0, 20, 0), glm::vec3(0, 40, 0), 2.0f, 0.0f, 1.0f));
	//allUp->addCommand(new MoveCommand(ship, glm::vec3(20, 30, 20), glm::vec3(20, 50, 20), 2.0f, 0.5f, 1.0f));
	script->pushCommand(allUp);
	return script;
}