#include "ScriptBuilder.h"
#include "MoveCommand.h"
#include "SerialCommand.h"
#include "ParallelCommand.h"
#include "RotateCommand.h"
#include "LookToward.h"
#include "FollowCommand.h"
#include "RideCurve.h"
#include "Scene.h"

iCommand* TestScript()
{
	Scene* theScene = Scene::getTheScene();
	aGameItem* ship = theScene->findItem("ship2");
	aGameItem* player = theScene->findItem("player");
	SerialCommand* script = new SerialCommand();
	ParallelCommand* allUp = new ParallelCommand();
	script->pushCommand(new RideCurve(ship, glm::vec3(-50, 30, 50), glm::vec3(50, 30, -50), glm::vec3(0, 120, 0), 12, 4, 0));
	//script->pushCommand(new LookToward(ship, glm::vec3(45,45,45), 5));
	//script->pushCommand(new MoveCommand(player, glm::vec3(10, 10, -10), glm::vec3(10, 10, -10), 0.00001f, 0.0f, 0.0f));
	//script->pushCommand(new MoveCommand(ship, glm::vec3(0, 34, -20), glm::vec3(20, 30, 20), 5.0f, 1.0f, 1.5f));
	//script->pushCommand(new MoveCommand(player, glm::vec3(10, 10, -10), glm::vec3(0, 20, 0), 2.0f, 0.0f, 1.0f));
	//allUp->addCommand(new FollowCommand(player, ship, glm::vec3(0,10,0), 20, 30, 60, 0));
	//allUp->addCommand(new MoveCommand(ship, ship->getPos() + glm::vec3(0, 50, 160), ship->getPos() - glm::vec3(0, 50, 160), 7, 2, 2));
	//allUp->addCommand(new RotateCommand(ship, glm::vec3(0, 0, 90), 1.0));
	//allUp->addCommand(new MoveCommand(player, glm::vec3(0, 20, 0), glm::vec3(0, 40, 0), 2.0f, 0.0f, 1.0f));
	//allUp->addCommand(new MoveCommand(ship, glm::vec3(20, 30, 20), glm::vec3(20, 50, 20), 2.0f, 0.5f, 1.0f));
	script->pushCommand(allUp);
	return script;
}